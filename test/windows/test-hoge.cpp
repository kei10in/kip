#include <Windows.h>
#include <Shlwapi.h>
#include <comdef.h>
#include <comip.h>

#include <boost/optional/optional_io.hpp>

#include "catch.hpp"

#include "kip/print-schema.hpp"
#include "kip/windows/dump.hpp"
#include "kip/windows/parser.hpp"
#include "kip/windows/print-ticket-provider.hpp"


namespace {


TEST_CASE("hoge") {
  kip::windows::print_ticket_provider ptp
   = kip::windows::print_ticket_provider::open("Microsoft XPS Document Writer");


  // WCHAR const* const path = L"test\\data\\print-capabilities.xml";
  // char const* const device_namespace = "http://schemas.microsoft.com/windows/printing/oemdriverpt/Printer";
  // IStreamPtr s;
  // com_raise_on_failure(SHCreateStreamOnFileW(path, STGM_READ, &s));
  //
  // auto pcd = kip::windows::parse_print_capabilities(s, device_namespace);
  //
  // auto pms = get_page_media_size(pcd);
  //
  //
  // auto ps = kip::windows::print_settings();
  //
  // auto const& opts = ps.media_size().options();
  // REQUIRE(opts.size() > 0);
  // REQUIRE(opts[0].name())
  // ps.media_size().selected_option() = opts[0].name();
  // ps.media_size().display_name():
  // ps.media_size().display_ui();
  // REQUIRE(opts[0].selected() == true);
  // REQUIRE(opts[0].display_name())

}


}  // unamed namespace
