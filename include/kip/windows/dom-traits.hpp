#ifndef KIP_MSXML_DOM_TRAITS_HPP_
#define KIP_MSXML_DOM_TRAITS_HPP_

#import <msxml6.dll>

#include <string>

#include "kip/windows/utils.hpp"
#include "kip/xml/dom-traits.hpp"


namespace kip {
namespace msxml {

struct tag {
  using document = MSXML2::IXMLDOMDocument3Ptr;
  using element = MSXML2::IXMLDOMElementPtr;
  using attribute = MSXML2::IXMLDOMAttributePtr;
};

}  // namespace msxml


namespace xml {


// search prefix related with uri
template <>
std::pair<std::string, bool>
find_prefix<msxml::tag>(
  MSXML2::IXMLDOMElementPtr element, std::string const& uri)
{
  MSXML2::IMXNamespaceManager nm(CLSID_MXNamespaceManager60);
  nm->pushNodeContext(element, TRUE);

  MSXML2::IMXNamespacePrefixes prefixes;
  nm->getPrefixes(to_bstr(uri), &prefixes);

  if (prefixes->length == 0) {
    return std::make_pair("", false);
  } else {
    return std::make_pair(from_bstr(prefixes->item[0], true));
  }
}


template <>
void set_document_element<msxml::tag>(
  MSXML2::IXMLDOMDocument3Ptr document, MSXML2::IXMLDOMElementPtr element)
{
  document->documentElement = element;
}


template <>
MSXML2::IXMLDOMElementPtr create_element_ns<msxml::tag>(
  MSXML2::IXMLDOMDocument3Ptr document, xml::qname const& name)
{
  return kip::detail::createElementNS(
    document, to_bstr(name.prefixed_name()), to_bstr(name.url));
}


template <>
MSXML2::IXMLDOMAttributePtr create_attribute<msxml::tag>(
  MSXML2::IXMLDOMDocument3Ptr document, std::string const& name)
{
  return document->createAttribute(to_bstr(name));
}


template <>
MSXML2::IXMLDOMAttributePtr create_attribute_ns<msxml::tag>(
  MSXML2::IXMLDOMDocument3Ptr document, xml::qname const& name)
{
  return kip::detail::createAttributeNS(
    document, to_bstr(name.prefixed_name()), to_bstr(name.url));
}


// Element node
template <>
MSXML2::IXMLDOMAttribute get_attribute_node<msxml::tag>(
  MSXML::IXMLDOMElementPtr elem, std::string const& name)
{
  return elem->getAttributeNode(to_bstr(name));
}


template <>
MSXML2::IXMLDOMAttributePtr set_attribute_node<msxml::tag>(
  MSXML2::IXMLDOMElementPtr elem, MSXML2::IXMLDOMAttributePtr attr)
{
  return elem->setAttributeNode(attr);
}


template <>
MSXML2::IXMLDOMElementPtr append_child<msxml::tag>(
  MSXML2::IXMLDOMElementPtr elem, MSXML2::IXMLDOMElementPtr new_child)
{
  return elem->appendChild(new_child);
}


template <>
void set_text<msxml::tag>(MSXML2::IXMLDOMElementPtr elem,
                          std::string const& text)
{
  elem->text = to_bstr(text);
}


// Attribute node
template <>
void set_value<msxml::tag>(MSXML2::IXMLDOMAttributePtr attr,
                           std::string const& value)
{
  attr->value = to_bstr(value);
}

template <>
void set_value<msxml::tag>(MSXML2::IXMLDOMAttributePtr attr,
                           xml::qname const& value)
{
  attr->value = to_bstr(value.prefixed_name());
}


}  // namespace xml
}  // namespace kip

#endif  // KIP_MSXML_DOM_TRAITS_HPP_
