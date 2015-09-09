#include "catch.hpp"

#include "kip/kip.hpp"


namespace {


TEST_CASE("construct print-capabilities") {
  using namespace kip;

  feature_impl ft0(psk::PageMediaSize);
  ft0.property(psf::SelectionType) = psk::PickOne;
  ft0.property(psk::DisplayName) = "Paper Size";

  REQUIRE(ft0.property(psf::SelectionType).as<xml::qname>() == psk::PickOne);
  //REQUIRE(ft0.property(psk::DisplayName).as<std::string>() == "Paper Size")

  // option op0(psk::ISOA4);
  // op0.property(psk::DisplayName) = "A4";
  // op0.scored_property(psk::MediaSizeWidth) = 210000;
  // op0.scored_property(psk::MediaSizeHeight) = 297000;
  // ft0.add(op0);
  //
  // option op1(psk::NorthAmericaLetter);
  // op1.property(psk::DisplayName) = "Letter";
  // op1.scored_property(psk::MediaSizeWidth) = 215900;
  // op1.scored_property(psk::MediaSizeHeight) = 279400;
  // ft1.add(op1);
  //
  // feature ft1(psk::JobNUpAllDocumentsContiguously);
  // ft1.property(psk::SelectionType) = psk::PickOne;
  // ft0.property(psk::DisplayName) = "N up";
  //
  // option op2;
  // op2.property(psk::DisplayName) = "2 up";
  // op2.scored_property(psk::PagesPerSheet) = 2;
  // ft1.add(op2);
  //
  // option op3;
  // op3.property(psk::DisplayName) = "4 up";
  // op3.scored_property(psk::PagesPerSheet) = 4;
  // ft1.add(op3);
  //
  // print_capabilities pc(1, "http://example.com/print-schema", "vnd");
  // pc.add(ft0);
  // pc.add(ft1);
}


}  // unamed namespace
