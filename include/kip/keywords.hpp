#ifndef KIP_KEYWORDS_HPP_
#define KIP_KEYWORDS_HPP_

#include "kip/xml/qname.hpp"

namespace kip {
namespace psk {

char const* const url = "http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords";
char const* const prefix = "psk";

xml::qname const DisplayName { "DisplayName", url };

xml::qname const None                { "None", url };
xml::qname const PrintTicketSettings { "PrintTicketSettings", url };
xml::qname const AdminSettings       { "AdminSettings", url };
xml::qname const DeviceSettings      { "DeviceSettings", url };


xml::qname const Unconditional { "Unconditional", url };
xml::qname const Conditional   { "Conditional",   url };
xml::qname const Optional      { "Optioal",       url };

// Media Size
xml::qname const PageMediaSize   { "PageMediaSize",   url };
xml::qname const MediaSizeWidth  { "MediaSizeWidth",  url };
xml::qname const MediaSizeHeight { "MediaSizeHeight", url };

xml::qname const ISOA4 { "ISOA4", url };
xml::qname const ISOA5 { "ISOA5", url };
xml::qname const NorthAmericaLetter { "NorthAmericaLetter", url };

// Copies
xml::qname const JobCopiesAllDocuments { "JobCopiesAllDocuments", url };

// N up
xml::qname const JobNUpAllDocumentsContiguously { "JobNUpAllDocumentsContiguously", url };
xml::qname const PagesPerSheet { "PagesPerSheet", url };
xml::qname const PresentationDirection { "PresentationDirection", url };


}  // namespace psk
}  // namespace pst

#endif  // KIP_KEYWORDS_HPP_
