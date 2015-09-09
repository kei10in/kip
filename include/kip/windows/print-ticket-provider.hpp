#ifndef KIP_WINDOWS_PRINT_TICKET_PROVIDER_HPP_
#define KIP_WINDOWS_PRINT_TICKET_PROVIDER_HPP_

#include <Prntvpt.h>

#include <memory>
#include <type_traits>

#include "kip/windows/print-ticket-provider.hpp"


namespace kip { namespace windows {

class print_ticket_provider {
  typedef std::shared_ptr<std::remove_pointer<HPTPROVIDER>::type> provider_t;
  provider_t provider;

  explicit print_ticket_provider(provider_t p)
    : provider(p)
  {}

public:
  print_ticket_provider() = default;
  print_ticket_provider(print_ticket_provider const& rhs) = default;
  print_ticket_provider& operator=(print_ticket_provider const& rhs) = default;

  // kip::xml::document print_capabilities() {
  //   MSXML2::IXMLDOMDocument3Ptr d(CLSID_DOMDocument60);
  //   BSTR error = nullptr;
  //
  //   HRESULT hr = PTGetPrintCapabilities(provider.get(), nullptr, d, &error);
  //   if (error != nullptr) SysFreeString(error);
  //
  //   if (SUCCEEDED(hr)) {
  //     return d;
  //   } else {
  //     _com_raise_error(hr);
  //   }
  // }

  static print_ticket_provider open(std::string const& name) {
    HPTPROVIDER hProvider = 0;
    DWORD usedVersion = 0;
    HRESULT hr = PTOpenProviderEx(detail::to_bstr(name), 1, 1, &hProvider, &usedVersion);

    if (FAILED(hr)) {

    }

    try {
      provider_t p(hProvider, PTCloseProvider);
      return print_ticket_provider(p);
    } catch (...) {
      PTCloseProvider(hProvider);
	  throw;
    }
  }
};

}}  // namespace kip::windows


#endif /* end of include guard: KIP_WINDOWS_PRINT_TICKET_PROVIDER_HPP_ */
