#ifndef KIP_XML_LITE_DEFS_HPP
#define KIP_XML_LITE_DEFS_HPP

#include <comdef.h>
#include <comip.h>


inline HRESULT com_raise_on_failure(HRESULT hr) {
  if (FAILED(hr))
    _com_raise_error(hr);
  return hr;
}


_COM_SMARTPTR_TYPEDEF(IXmlReader, __uuidof(IXmlReader));
_COM_SMARTPTR_TYPEDEF(IXmlWriter, __uuidof(IXmlWriter));


#endif  // KIP_XML_LITE_DEFS_HPP
