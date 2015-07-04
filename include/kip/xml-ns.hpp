#ifndef KIP_XML_NS_HPP_
#define KIP_XML_NS_HPP_

#include "kip/xml/qname.hpp"

namespace kip {

namespace xsi {

char const* const url = "http://www.w3.org/2001/XMLSchema-instance";
char const* const prefix = "xsi";

xml::qname const type { "type", url };

}  // namespace xsi

namespace xsd {

char const* const url = "http://www.w3.org/2001/XMLSchema";
char const* const prefix = "xsd";

xml::qname const decimal { "decimal", url };
xml::qname const integer { "integer", url };
xml::qname const string  { "string",  url };
xml::qname const QName   { "QName",   url };

}  // namespace xsd

}  // namespace kip

#endif
