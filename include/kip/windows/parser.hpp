#ifndef KIP_WINDOWS_PARSER_HPP_
#define KIP_WINDOWS_PARSER_HPP_

#include <XmlLite.h>

#include <string>

#include <boost/algorithm/string.hpp>

#include "kip/core/elements.hpp"
#include "kip/windows/defs.hpp"
#include "kip/windows/utils.hpp"


// switch (nt) {
//  case XmlNodeType_None: break;
//  case XmlNodeType_Element:
//   return parse_print_schema_document();
//  case XmlNodeType_Attribute:
//  case XmlNodeType_Text:
//  case XmlNodeType_CDATA:
//  case XmlNodeType_ProcessingInstruction: break;
//  case XmlNodeType_Comment: break;
//  case XmlNodeType_DocumentType: break;
//  case XmlNodeType_Whitespace: break;
//  case XmlNodeType_EndElement: break;
//  case XmlNodeType_XmlDeclaration: break;
//  default: break;
// }


namespace kip {
namespace windows {


class parser {
  IXmlReaderPtr reader;
  std::string device_namespace_;
  std::unordered_map<std::string, std::string> namespace_context;

 public:
  parser(IXmlReader* r, std::string const& dn)
    : reader(r)
    , device_namespace_(dn)
  {}

  std::string const& device_namespace() const { return device_namespace_; }
  std::string& device_namespace() { return device_namespace_; }

  boost::optional<print_capabilities_document> parse_print_capabilities() {
    auto const tag = to_first_element();
    if (tag != kip::psf::PrintCapabilities)
      return boost::none;

    parse_namespace_declaration();

    boost::optional<int> version = get_version_attribute();
    if (!version) return boost::none;

    print_capabilities_document result {
       *version, device_namespace(), device_namespace_prefix() };

    enumerate_children([this, &result](xml::qname const& tag) {
      if (tag == psf::Feature) {
        auto ft = parse_feature();
        if (ft) {
          result.features.insert(*ft);
        } else {
          // todo: implementerror handling
        }
      } else if (tag == psf::Property) {
        auto p = parse_property();
        if (p) {
          result.properties.insert(*p);
        } else {
          // todo: implement error handling
        }
      } else if (tag == psf::ParameterDef) {
        auto pd = parse_parameter_def();
        if (pd) {
          result.parameters.insert(*pd);
        } else {
          // todo: implement error handling
        }
      } else {
        // todo: parameter init / property
        skip_subtree();
      }
    });

    return result;
  }

private:
  boost::optional<xml::qname> to_first_element() {
    XmlNodeType nt;
    while (S_OK == reader->Read(&nt)) {
      if (nt == XmlNodeType_Element) {
        break;
      } else if (nt == XmlNodeType_ProcessingInstruction ||
                 nt == XmlNodeType_Comment ||
                 nt == XmlNodeType_DocumentType ||
                 nt == XmlNodeType_Whitespace ||
                 nt == XmlNodeType_XmlDeclaration) {
        // do nothing
      } else if (nt == XmlNodeType_None) {
        return boost::none;
      } else {  // no elements
        return boost::none;
      }
    }

    return get_qname();
  }

  boost::optional<std::pair<xml::qname, feature>> parse_feature() {
    auto const name = get_name_attribute();
    if (!name) return boost::none;
    auto const& name_ = *name;

    feature ft;

    enumerate_children([this, &ft](xml::qname const& tag) {
      if (tag == psf::Option) {
        auto op = parse_option();
        if (op) {
          ft.options.push_back(std::move(*op));
        } else {
          // todo: implement error handling
        }
      } else if (tag == psf::Property) {
        auto p = parse_property();
        if (p) {
          ft.properties.insert(*p);
        } else {
          // todo: implement error handling
        }
      } else if (tag == psf::Feature) {
        auto sft = parse_feature();
        if (sft) {
          ft.sub_features.insert(*sft);
        } else {
          // todo: implement error handling
        }
      } else {
          // todo: property / sub feature
        skip_subtree();
      }
    });

    return std::make_pair(name_, ft);
  }

  boost::optional<option> parse_option() {
    auto const name = get_name_attribute();
    auto const constrained = get_constrained_attribute();

    option op { name, constrained };

    enumerate_children([this, &op](xml::qname const& tag) {
      if (tag == psf::ScoredProperty) {
        auto sp = parse_scored_property();
        if (sp) {
          op.scored_properties.insert(*sp);
        } else {
          // todo: implement error handling
        }
      } else if (tag == psf::Property) {
        auto p = parse_property();
        if (p) {
          op.properties.insert(*p);
        } else {
          // todo: implement error handling
        }
      } else {
        skip_subtree();
      }
    });

    return op;
  }

  boost::optional<std::pair<xml::qname, parameter_def>> parse_parameter_def() {
    auto const name = get_name_attribute();
    if (!name) return boost::none;

    parameter_def pd;

    enumerate_children([this, &pd](xml::qname const& tag) {
      if (tag == psf::Property) {
        auto p = parse_property();
        if (p) {
          pd.properties.insert(*p);
        } else {
          // todo: implement error handling
        }
      } else {
        skip_subtree();
      }
    });

    return std::make_pair(*name, pd);
  }

  boost::optional<parameter_init> parse_parameter_init() {
    auto const name = get_name_attribute();
    if (!name) return boost::none;

    parameter_init pi;

    enumerate_children([this, &pi](xml::qname const& tag) {
      if (tag == psf::Value) {
        auto v = parse_value();
        if (v) {
          pi.value = v;
        } else {
          // todo: implement error handling
        }
      } else {
        skip_subtree();
      }
    });

    return pi;
  }

  boost::optional<std::pair<xml::qname, scored_property>>
  parse_scored_property() {
    auto const name = get_name_attribute();
    if (!name) return boost::none;

    scored_property sp;

    enumerate_children([this, &sp](xml::qname const& tag) {
      if (tag == psf::Value) {
        auto value = parse_value();
        if (value) {
          sp.value = *value;
        } else {
          // todo: error handling
        }
      } else if (tag == psf::ParameterRef) {
        auto pr = parse_parameter_ref();
        if (pr) {
          sp.content = *pr;
        } else {
          // todo: error handling
        }
      } else {
        // todo: implement parsing nested properties
        // todo: implement parsing nested scored properties
        skip_subtree();
      }
    });

    return std::make_pair(*name, sp);
  }

  boost::optional<std::pair<xml::qname, property>> parse_property() {
    auto const name = get_name_attribute();
    if (!name) return boost::none;
	auto const& name_ = *name;

    property prop;

    enumerate_children([this, &prop](xml::qname const& tag) {
      if (tag == psf::Value) {
        auto value = parse_value();
        if (value) {
          prop.value = *value;
        } else {
          // todo: error handling
        }
      } else if (tag == psf::Property) {
        auto p = parse_property();
        if (p) {
          prop.properties.insert(*p);
        } else {
          // todo: error handling
        }
      } else {
        skip_subtree();
      }
    });

    return std::make_pair(name_, prop);
  }

  boost::optional<parameter_ref> parse_parameter_ref() {
    auto const name = get_name_attribute();
    if (!name) return boost::none;

    skip_subtree();

    return parameter_ref { *name };
  }

  boost::optional<variable> parse_value() {
    HRESULT const hr = com_raise_on_failure(
      move_to_attribute_by_name(xsi::type));
    if (hr != S_OK) return boost::none;

    auto type = to_qname(get_value());

    boost::optional<variable> result;

    XmlNodeType nt;
    while (S_OK == reader->Read(&nt)) {
      if (nt == XmlNodeType_Element) {
        skip_subtree();
      } else if (nt == XmlNodeType_EndElement) {
        break;
      } else if (nt == XmlNodeType_Text) {
        std::string const text = get_value();
        if (type == xsd::string) {
          result = variable{ text };
        } else if (type == xsd::QName) {
          auto const qname = to_qname(text);
          if (qname) {
            result = variable{ *qname };
          } else {
            result = variable{ xml::qname { text, "" } };
          }
        } else if (type == xsd::integer) {
          result = variable{ std::stoi(text) };
        } else if (type == xsd::decimal) {
          result = variable{ std::stof(text) };
        }
      }
    }

    return result;
  }

  template <class Func>
  void enumerate_children(Func f) {
    reader->MoveToElement();
    if (reader->IsEmptyElement()) return;

    XmlNodeType nt;
    while (S_OK == reader->Read(&nt)) {
      if (nt == XmlNodeType_Element) {
        xml::qname const tag = get_qname();
        f(tag);
      } else if (nt == XmlNodeType_EndElement) {
        break;
      }
    }
  }

  void skip_subtree() {
    reader->MoveToElement();
    if (reader->IsEmptyElement()) return;

    XmlNodeType nt;
    while (S_OK == reader->Read(&nt)) {
      if (nt == XmlNodeType_Element) {
        skip_subtree();
      } else if (nt == XmlNodeType_EndElement) {
        break;
      }
    }
  }

  std::string device_namespace_prefix() {
    HRESULT hr = reader->MoveToFirstAttribute();
    while (hr == S_OK) {  // todo: detect E_PENDING
      auto const pre = get_prefix();
      if (pre == "xmlns") {
        auto const ln = get_local_name();
        auto const url = get_value();
        if (url == device_namespace()) {
          return ln;
        }
      }
	  hr = reader->MoveToNextAttribute();
    }
  }

  void parse_namespace_declaration() {
    HRESULT hr = reader->MoveToFirstAttribute();
    while (hr == S_OK) {  // todo: detect E_PENDING
      auto const pre = get_prefix();
      if (pre == "xmlns") {
        auto const ln = get_local_name();
        auto const url = get_value();
        namespace_context.insert(std::make_pair(ln, url));
      }
	  hr = reader->MoveToNextAttribute();
    }
  }

  boost::optional<int> get_version_attribute() {
    HRESULT const hr = com_raise_on_failure(
      reader->MoveToAttributeByName(L"version", nullptr));
    if (hr == S_OK) {
      std::istringstream iss(get_value());
      int i = 0;
      iss >> i;
      if (iss) return i;
    }
    return boost::none;
  }

  boost::optional<xml::qname> get_name_attribute() {
    HRESULT const hr = com_raise_on_failure(
      reader->MoveToAttributeByName(L"name", nullptr));
    if (hr == S_OK) {
      return to_qname(get_value());
    } else {
      return boost::none;
    }
  }

  boost::optional<xml::qname> get_constrained_attribute() {
    HRESULT const hr = com_raise_on_failure(
      reader->MoveToAttributeByName(L"constrained", nullptr));
    if (hr == S_OK) {
      return to_qname(get_value());
    } else {
      return boost::none;
    }
  }

  boost::optional<xml::qname> to_qname(std::string const& name) {
    auto const p = name.find(":");
    // todo: preadicate which p is npos
    auto const prefix = name.substr(0, p);
    auto const it = namespace_context.find(prefix);

    if (it == namespace_context.end()) return boost::none;

    return xml::qname { name.substr(p + 1),  it->second };
  }

  HRESULT move_to_attribute_by_name(xml::qname const& name) {
    std::wstring ln = detail::to_utf16(name.name);
    std::wstring url = detail::to_utf16(name.url);

    return reader->MoveToAttributeByName(ln.c_str(), url.c_str());
  }

  xml::qname get_qname() {
    return detail::get_qname(reader);
  }

  std::string get_namespace_uri() {
    return detail::get_namespace_uri(reader);
  }

  std::string get_prefix() {
    return detail::get_prefix(reader);
  }

  std::string get_local_name() {
    return detail::get_local_name(reader);
  }

  std::string get_value() {
    return detail::get_value(reader);
  }
};


boost::optional<print_capabilities_document> parse_print_capabilities(
  IXmlReader* r, std::string const& device_namespace) {
  parser p(r, device_namespace);
  return p.parse_print_capabilities();
}


boost::optional<print_capabilities_document> parse_print_capabilities(
  IStream* s, std::string const& device_namespace) {
  IXmlReaderPtr r;
  com_raise_on_failure(CreateXmlReader(__uuidof(IXmlReader), (void**)&r, nullptr));
  com_raise_on_failure(r->SetInput(s));
  return parse_print_capabilities(r, device_namespace);
}


}  // xmllite
}  // namespace kip


#endif  // KIP_WINDOWS_PARSER_HPP_
