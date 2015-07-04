#ifndef PST_MSXML_UTILS_HPP_
#define PST_MSXML_UTILS_HPP_

#import <msxml6.dll>
#include <xmllite.h>

#include <codecvt>
#include <locale>
#include <string>

#include <kip/xml/qname.hpp>


namespace kip { namespace windows { namespace detail {


inline std::string to_utf8(std::wstring const& ws)
{
  std::wstring_convert<
    std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::little_endian>,
    wchar_t> utf16le_conv;
  return utf16le_conv.to_bytes(ws.data());
}


inline std::wstring to_utf16(std::string const& s)
{
  std::wstring_convert<
    std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::little_endian>,
    wchar_t> utf16le_conv;
  return utf16le_conv.from_bytes(s.data());
}


inline _bstr_t to_bstr(std::string const& s)
{
  std::wstring ws = to_utf16(s);
  return ws.c_str();
}


inline std::string from_bstr(_bstr_t const& s)
{
  std::wstring ws(s);
  return to_utf8(ws);
}


template <class Func>
std::string by_string(IXmlReader* reader, Func f) {
  WCHAR const* p;
  (reader->*f)(&p, nullptr);
  return detail::to_utf8(p);
}

inline std::string get_namespace_uri(IXmlReader* reader) {
  return by_string(reader, &IXmlReader::GetNamespaceUri);
}

inline std::string get_prefix(IXmlReader* reader) {
  return by_string(reader, &IXmlReader::GetPrefix);
}

inline std::string get_local_name(IXmlReader* reader) {
  return by_string(reader, &IXmlReader::GetLocalName);
}

inline std::string get_value(IXmlReader* reader) {
  return by_string(reader, &IXmlReader::GetValue);
}

inline xml::qname get_qname(IXmlReader* reader) {
  return xml::qname {
    get_local_name(reader), get_namespace_uri(reader)
    };
}


inline MSXML2::IXMLDOMElementPtr createElementNS(
  MSXML2::IXMLDOMDocument3Ptr document,
  _bstr_t const& name,
  _bstr_t const& namespaceURI)
{
  MSXML2::IXMLDOMNodePtr node = document->createNode(
    L"Element", name, namespaceURI);
  return node;
}


inline MSXML2::IXMLDOMAttributePtr createAttributeNS(
  MSXML2::IXMLDOMDocument3Ptr document,
  _bstr_t const& name,
  _bstr_t const& namespaceURI)
{
  MSXML2::IXMLDOMNodePtr node = document->createNode(
    L"Attribute", name, namespaceURI);
  return node;
}


}  // namespapc detail
}  // namespace windows
}  // namespace kip

#endif
