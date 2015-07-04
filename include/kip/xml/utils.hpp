#ifndef KIP_XML_UTILS_HPP_
#define KIP_XML_UTILS_HPP_

namespace kip {
namespace xml {

template <class T, class U>
xml::attribute<T> create_attribute(
  xml::document<T> d, std::string const& name, U const& value)
{
  auto a = create_attribute<T>(d, name);
  xml::set_value<T>(a, value);
  return a;
}


template <class T, class U>
xml::attribute<T> create_attribute_ns(
  xml::document<T> d, xml::qname const& name, U const& value)
{
  auto a = create_attribute_ns<T>(d, name);
  xml::set_value<T>(a, value);
  return a;
}


}  // namespace xml
}  // namespace kip

#endif  // KIP_XML_UTILS_HPP_
