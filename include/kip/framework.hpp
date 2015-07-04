#ifndef KIP_FRAMEWORK_HPP_
#define KIP_FRAMEWORK_HPP_

#include "kip/xml/qname.hpp"

namespace kip {
namespace psf {

constexpr char const* url = "http://schemas.microsoft.com/windows/2003/08/printing/printschemaframework";
char const* const prefix = "psf";

xml::qname const PrintTicket       { "PrintTicket",       url };
xml::qname const PrintCapabilities { "PrintCapabilities", url };
xml::qname const Feature           { "Feature",           url };
xml::qname const Option            { "Option",            url };
xml::qname const ParameterDef      { "ParameterDef",      url };
xml::qname const ParameterInit     { "ParameterInit",     url };
xml::qname const ParameterRef      { "ParameterRef",      url };
xml::qname const Property          { "Property",          url };
xml::qname const ScoredProperty    { "ScoredProperty",    url };
xml::qname const Value             { "Value",             url };


xml::qname const DataType     { "DataType",     url };
xml::qname const DefaultValue { "DefaultValue", url };
xml::qname const MaxValue     { "MaxValue",     url };
xml::qname const MinValue     { "MinValue",     url };
xml::qname const MaxLength    { "MaxLength",    url };
xml::qname const MinLength    { "MinLength",    url };
xml::qname const Mandatory    { "Mandatory",    url };
xml::qname const Multiple     { "Multiple",     url };
xml::qname const UnitType     { "UnitType",     url };


}  // namespace psf
}  // namespace pst

#endif  // KIP_FRAMEWORK_HPP_
