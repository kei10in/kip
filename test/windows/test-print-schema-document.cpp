#include <Windows.h>
#include <Shlwapi.h>
#include <comdef.h>
#include <comip.h>

#include <boost/optional/optional_io.hpp>

#include "catch.hpp"

#include "kip/print-schema.hpp"
#include "kip/windows/dump.hpp"
#include "kip/windows/parser.hpp"


namespace {


TEST_CASE("Read Print Capabilities document") {
  WCHAR const* const path = L"test\\data\\print-capabilities.xml";
  char const* const device_namespace = "http://schemas.microsoft.com/windows/printing/oemdriverpt/Printer";
  IStreamPtr s;
  com_raise_on_failure(SHCreateStreamOnFileW(path, STGM_READ, &s));

  auto pcd = kip::windows::parse_print_capabilities(s, device_namespace);
  REQUIRE(pcd);

  SECTION("read feature: PageMediaSize") {
    REQUIRE(pcd->features.size() == 11);
    auto const& ft = pcd->features.find(kip::psk::PageMediaSize);
    REQUIRE(ft != pcd->features.end());

    SECTION("read option") {
      REQUIRE(ft->second.options.size() > 0);
      auto const& op = ft->second.options[0];
      REQUIRE(op.name);
      REQUIRE(*op.name == kip::psk::NorthAmericaLetter);
      REQUIRE(op.constrained);
      REQUIRE(*op.constrained == kip::psk::None);

      SECTION("read scored property") {
        REQUIRE(op.scored_properties.size() > 0);
        auto sp = op.scored_properties.find(kip::psk::MediaSizeWidth);
        REQUIRE(sp != op.scored_properties.end());
        REQUIRE(sp->second.value);
        REQUIRE(*sp->second.value == 215900);
      }
    }
  }

  SECTION("read feature: JobNUpAllDocumentsContiguously") {
    REQUIRE(pcd->features.size() > 0);
    auto const& ft = pcd->features.find(kip::psk::JobNUpAllDocumentsContiguously);
    REQUIRE(ft != pcd->features.end());

    SECTION("read option") {
      REQUIRE(ft->second.options.size() == 6);
      auto const& op = ft->second.options[0];

      SECTION("read scored property") {
        REQUIRE(op.scored_properties.size() > 0);
        auto sp = op.scored_properties.find(kip::psk::PagesPerSheet);
        REQUIRE(sp != op.scored_properties.end());
        REQUIRE(sp->second.value);
        REQUIRE(*sp->second.value == 1);
      }

      SECTION("read property") {
        REQUIRE(op.properties.size() > 0);
        auto p = op.properties.find(kip::psk::DisplayName);
        REQUIRE(p != op.properties.end());
        REQUIRE(p->second.value);
        REQUIRE(*p->second.value == "1");
      }
    }

    SECTION("read property of feature") {
      REQUIRE(ft->second.properties.size() > 0);
      auto p = ft->second.properties.find(kip::psk::DisplayName);
      REQUIRE(p != ft->second.properties.end());
      REQUIRE(p->second.value);
      REQUIRE(*p->second.value == "Pages per Sheet");
    }

    SECTION("read sub feature") {
      REQUIRE(ft->second.sub_features.size() > 0);
      auto sft = ft->second.sub_features.find(kip::psk::PresentationDirection);
      REQUIRE(sft != ft->second.sub_features.end());
      REQUIRE(sft->first == kip::psk::PresentationDirection);
    }
  }

  SECTION("read parameter: copies") {
    auto p = pcd->parameters.find(kip::psk::JobCopiesAllDocuments);
    REQUIRE(p != pcd->parameters.end());
    auto const& pd = p->second;
    auto const& dt = pd.properties.find(kip::psf::DataType)->second;
    REQUIRE(*dt.value == kip::xsd::integer);

    auto const& mult = pd.properties.find(kip::psf::Multiple);
    REQUIRE(mult != pd.properties.end());
    REQUIRE(*mult->second.value == 1);

    auto const& mini = pd.properties.find(kip::psf::MinValue);
    REQUIRE(mini != pd.properties.end());
    REQUIRE(*mini->second.value == 1);

    auto const& maxi = pd.properties.find(kip::psf::MaxValue);
    REQUIRE(maxi != pd.properties.end());
    REQUIRE(*maxi->second.value == 9999);

    auto const& dv = pd.properties.find(kip::psf::DefaultValue);
    REQUIRE(dv != pd.properties.end());
    REQUIRE(*dv->second.value == 1);

    auto const& ma = pd.properties.find(kip::psf::Mandatory);
    REQUIRE(ma != pd.properties.end());
    REQUIRE(*ma->second.value == kip::psk::Unconditional);

    auto const& ut = pd.properties.find(kip::psf::UnitType);
    REQUIRE(ut != pd.properties.end());
    REQUIRE(*ut->second.value == "copies");
  }
}


TEST_CASE("Write") {
  WCHAR const* const path = L"test\\data\\print-capabilities.xml";
  char const* const device_namespace = "http://schemas.microsoft.com/windows/printing/oemdriverpt/Printer";

  IStreamPtr is;
  com_raise_on_failure(SHCreateStreamOnFileW(path, STGM_READ, &is));

  auto pcd = kip::windows::parse_print_capabilities(is, device_namespace);

  IStreamPtr os;
  os.Attach(SHCreateMemStream(nullptr, 0));
  kip::windows::dump(*pcd, os);

  LARGE_INTEGER pos = {};
  os->Seek(pos, STREAM_SEEK_SET, nullptr);

  auto pcd2 = kip::windows::parse_print_capabilities(os, device_namespace);

  std::string const s1 = kip::windows::dump(*pcd);
  std::string const s2 = kip::windows::dump(*pcd2);
  REQUIRE(s1 == s2);
  
  REQUIRE(pcd->version == pcd2->version);
  REQUIRE(pcd->device_namespace == pcd2->device_namespace);
  REQUIRE(pcd->features.size() == pcd2->features.size());
  REQUIRE(pcd->properties.size() == pcd2->properties.size());
  REQUIRE(pcd->parameters.size() == pcd2->parameters.size());
}



}  // unamed namespace
