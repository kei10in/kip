#include "catch.hpp"

#include "kip/core/elements.hpp"
#include "kip/keywords.hpp"


namespace {


TEST_CASE("printschema-document is factory of printschema elements") {
  kip::print_schema_document psd;

  kip::option op { kip::psk::ISOA4 };
  op.scored_properties.emplace(kip::psk::MediaSizeWidth, 210000);
  op.scored_properties.emplace(kip::psk::MediaSizeHeight, 297000);

  kip::feature ft { { op } };

  psd.features.emplace(kip::psk::PageMediaSize, ft);

  auto it = psd.features.find(kip::psk::PageMediaSize);
  REQUIRE(it != psd.features.end());
  REQUIRE(it->first == kip::psk::PageMediaSize);
}


}  // unamed namespace
