#ifndef KIP_XML_DOM_TRAITS_HPP_
#define KIP_XML_DOM_TRAITS_HPP_

#include "kip/xml/qname.hpp"

namespace kip {
namespace xml {

template <class T>
struct dom_traits {
  using document = typename T::document;
  using element = typename T::element;
  using attribute = typename T::attribute;
};

template <class T> using document  = typename dom_traits<T>::document;
template <class T> using element   = typename dom_traits<T>::element;
template <class T> using attribute = typename dom_traits<T>::attribute;


template <class T>
void set_document_element(document<T> document, element<T> element);


template <class T>
element<T> create_element_ns(document<T> document,
                             xml::qname const& name);


template <class T>
attribute<T> create_attribute(document<T> document, std::string const& name);


template <class T>
attribute<T> create_attribute_ns(document<T> document,
                                 xml::qname const& name);


// Element node
template <class T>
attribute<T> set_attribute_node(element<T> elem, attribute<T> attr);


template <class T>
element<T> append_child(element<T> elem, element<T> new_child);


template <class T>
void set_text(element<T> elem, std::string const& text);


// Attribute node
template <class T>
void set_value(attribute<T> attr, std::string const& value);


template <class T>
void set_value(attribute<T> attr, xml::qname const& value);

}  // namespace xml
}  // namespace kip

#endif  // KIP_XML_DOM_TRAITS_HPP_
