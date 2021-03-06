#ifndef KIP_WINDOWS_DUMP_HPP_
#define KIP_WINDOWS_DUMP_HPP_

#include <XmlLite.h>

#include <algorithm>
#include <iterator>
#include <string>

#include "kip/elements.hpp"
#include "kip/windows/defs.hpp"
#include "kip/windows/utils.hpp"
#include "kip/xml-ns.hpp"

namespace kip {
namespace windows {
namespace detail {

class xml_writer {
  IStreamPtr s;
  IXmlWriterPtr w;
  std::unordered_map<std::string, std::string> ns_decls;

public:
  explicit xml_writer(IStream* out) : s(out) {
    com_raise_on_failure(CreateXmlWriter(__uuidof(IXmlWriter), (void**)&w, nullptr));
    com_raise_on_failure(w->SetOutput(s));
    com_raise_on_failure(w->SetProperty(XmlWriterProperty_Indent, TRUE));
  }

  void start_document() {
    com_raise_on_failure(w->WriteStartDocument(XmlStandalone_Omit));
  }

  void end_document() {
    com_raise_on_failure(w->WriteEndDocument());
  }

  void start_element(std::string const& prefix,
                     xml::qname const& tag) {
    _bstr_t const p = to_bstr(prefix);
    _bstr_t const l = to_bstr(tag.name);
    _bstr_t const u = to_bstr(tag.url);
    com_raise_on_failure(w->WriteStartElement(p, l, u));
  }

  void start_element(xml::qname const& tag) {
    _bstr_t const l = to_bstr(tag.name);
    _bstr_t const u = to_bstr(tag.url);
    com_raise_on_failure(w->WriteStartElement(nullptr, l, u));
  }

  void end_element() {
    com_raise_on_failure(w->WriteEndElement());
  }

  void xmlns(std::string const& prefix, std::string const& url) {
    ns_decls.insert(std::make_pair(url, prefix));

    _bstr_t const ln = to_bstr(prefix);
    _bstr_t const v = to_bstr(url);
    com_raise_on_failure(w->WriteAttributeString(L"xmlns", ln, nullptr, v));
  }

  void attribute(std::string const& name, int value) {
    _bstr_t n = to_bstr(name);
    _bstr_t v = to_bstr(std::to_string(value));
    com_raise_on_failure(w->WriteAttributeString(nullptr, n, nullptr, v));
  }

  void attribute(std::string const& name, xml::qname const& value) {
    _bstr_t n = to_bstr(name);
    auto pn = to_prefixed_name(value);
    if (!pn) {
      // todo: implement error handling
      return;
    }
    _bstr_t v = to_bstr(*pn);
    com_raise_on_failure(w->WriteAttributeString(nullptr, n, nullptr, v));
  }

  void attribute(xml::qname const& name, xml::qname const& value) {
    _bstr_t l = to_bstr(name.name);
    _bstr_t u = to_bstr(name.url);
    auto pn = to_prefixed_name(value);
    if (!pn) {
      // todo: implement error handling
      return;
    }
    _bstr_t v = to_bstr(*pn);
    com_raise_on_failure(w->WriteAttributeString(nullptr, l, u, v));
  }

  void text(std::string const& t) {
    _bstr_t s = to_bstr(t);
    com_raise_on_failure(w->WriteString(s));
  }

  void text(xml::qname const& text) {
    _bstr_t l = to_bstr(text.name);
    _bstr_t u = to_bstr(text.url);
    com_raise_on_failure(w->WriteQualifiedName(l, u));
  }

  void text(int v) {
    text(std::to_string(v));
  }

  void text(float v) {
    text(std::to_string(v));
  }

  boost::optional<std::string> to_prefixed_name(xml::qname const& value) {
    auto it = ns_decls.find(value.url);
    if (it == ns_decls.end()) return boost::none;
    return it->second + ":" + value.name;
  }

  void flush() {
    w->Flush();
  }
};

inline xml::qname to_qname(value_type vt) {
  switch (vt) {
    default:
    case value_type::string:  return xsd::string;
    case value_type::qname:   return xsd::QName;
    case value_type::integer: return xsd::integer;
    case value_type::decimal: return xsd::decimal;
  }
}


class serializer {
  xml_writer w;

public:
  serializer(IStream* s)
	: w(s)
  {}

  void start(print_capabilities const& pcd) {
    w.start_document();
    w.start_element(psf::prefix, psf::PrintCapabilities);

    w.attribute("version", pcd.version());

    w.xmlns(psf::prefix, psf::url);
    w.xmlns(psk::prefix, psk::url);
    w.xmlns(xsi::prefix, xsi::url);
    w.xmlns(xsd::prefix, xsd::url);
    w.xmlns(pcd.device_namespace_prefix(), pcd.device_namespace());
  }

  void end(print_capabilities const& pcd) {
    w.end_element();
    w.end_document();
	w.flush();
  }

  void start(feature_def const& ft) {
    w.start_element(psf::Feature);
    w.attribute("name", ft.name());
  }

  void start(option const& o) {
    w.start_element(psf::Option);
    if (!o.name().empty()) {
      w.attribute("name", o.name());
    }
  }

  void start(parameter_def const& pd) {
    w.start_element(psf::ParameterDef);
    w.attribute("name", pd.name());
  }

  void start(property const& p) {
    w.start_element(psf::Property);
    w.attribute("name", p.name());
  }

  void start(scored_property const& sp) {
    w.start_element(psf::ScoredProperty);
    w.attribute("name", sp.name());
  }

  void start(value const& v) {
    w.start_element(psf::Value);
    w.attribute(xsi::type, to_qname(v.type()));
    switch (v.type()) {
    case value_type::string:
      w.text(v.string_value());
      break;
    case value_type::qname:
      w.text(v.qname_value());
      break;
    case value_type::integer:
      w.text(v.integer_value());
      break;
    case value_type::decimal:
      w.text(v.decimal_value());
      break;
    }
  }

  template <class T>
  void end(T const& e) {
    w.end_element();
  }
};


}  // namespace detail


inline void dump(print_capabilities const& pcd, IStream* out) {
  detail::serializer s(out);
  pcd.traverse(s);
}


inline std::string dump(print_capabilities const& pcd) {
  IStreamPtr out;
  out.Attach(SHCreateMemStream(nullptr, 0));

  dump(pcd, out);

  LARGE_INTEGER pos = {};
  out->Seek(pos, STREAM_SEEK_SET, nullptr);
  std::string result;
  for (;;) {
    char buf[1024] = {};
    ULONG read = 0;
    out->Read(buf, sizeof(buf), &read);

    std::copy(buf, buf + read, std::back_inserter(result));

    if (read < sizeof(buf)) break;
  }

  return result;
}


}  // namespace windows
}  // namespace kip


#endif  // KIP_WINDOWS_DUMP_HPP_
