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
  REQUIRE(!pcd.empty());

  SECTION("read feature: PageMediaSize") {
	REQUIRE(pcd.features().size() == 11);

	auto ft = pcd.feature(kip::psk::PageMediaSize);
	REQUIRE(ft.empty() == false);

	SECTION("read option") {
	  REQUIRE(ft.options().size() > 0);
	  auto op = ft.options()[0];
	  REQUIRE(op.name());
	  REQUIRE(*op.name() == kip::psk::NorthAmericaLetter);
	  REQUIRE(op.constrained());
	  REQUIRE(*op.constrained() == kip::psk::None);

	  SECTION("read scored property") {
		REQUIRE(op.scored_properties().size() > 0);
		auto sp = op.scored_property(kip::psk::MediaSizeWidth);
		REQUIRE(sp.empty() == false);
		REQUIRE(sp.value() == 215900);
	  }
	}
  }

  SECTION("read feature: JobNUpAllDocumentsContiguously") {
	REQUIRE(pcd.features().size() > 0);
	auto ft = pcd.feature(kip::psk::JobNUpAllDocumentsContiguously);
	REQUIRE(ft.empty() == false);

	SECTION("read option") {
	  REQUIRE(ft.options().size() == 6);
	  auto op = ft.options()[0];

	  SECTION("read scored property") {
		REQUIRE(op.scored_properties().size() > 0);
		auto sp = op.scored_property(kip::psk::PagesPerSheet);
		REQUIRE(sp.empty() == false);
		REQUIRE(sp.value() == 1);
	  }

	  SECTION("read property") {
		REQUIRE(op.properties().size() > 0);
		auto p = op.property(kip::psk::DisplayName);
		REQUIRE(p.empty() == false);
		REQUIRE(p.value() == "1");
	  }
	}

	SECTION("read property of feature") {
	  REQUIRE(ft.properties().size() > 0);
	  auto p = ft.property(kip::psk::DisplayName);
	  REQUIRE(p.empty() == false);
	  REQUIRE(p.value() == "Pages per Sheet");
	}

	SECTION("read sub feature") {
	  REQUIRE(ft.sub_features().size() > 0);
	  auto sft = ft.sub_feature(kip::psk::PresentationDirection);
	  REQUIRE(sft.empty() == false);
	  REQUIRE(sft.name() == kip::psk::PresentationDirection);
	}
  }

  SECTION("read parameter: copies") {
	auto p = pcd.parameter(kip::psk::JobCopiesAllDocuments);
	REQUIRE(p.empty() == false);

	REQUIRE(p.property(kip::psf::DataType).value() == kip::xsd::integer);
	REQUIRE(p.property(kip::psf::Multiple).value() == 1);
	REQUIRE(p.property(kip::psf::MinValue).value() == 1);
	REQUIRE(p.property(kip::psf::MaxValue).value() == 9999);
	REQUIRE(p.property(kip::psf::DefaultValue).value() == 1);
	REQUIRE(p.property(kip::psf::Mandatory).value() == kip::psk::Unconditional);
	REQUIRE(p.property(kip::psf::UnitType).value() == "copies");
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
  kip::windows::dump(pcd, os);

  LARGE_INTEGER pos = {};
  os->Seek(pos, STREAM_SEEK_SET, nullptr);

  auto pcd2 = kip::windows::parse_print_capabilities(os, device_namespace);

  std::string const s1 = kip::windows::dump(pcd);
  std::string const s2 = kip::windows::dump(pcd2);

  REQUIRE(pcd.version() == pcd2.version());
  REQUIRE(pcd.device_namespace() == pcd2.device_namespace());
  REQUIRE(pcd.features().size() == pcd2.features().size());
  REQUIRE(pcd.properties().size() == pcd2.properties().size());
  REQUIRE(pcd.parameters().size() == pcd2.parameters().size());
}



}  // unamed namespace
