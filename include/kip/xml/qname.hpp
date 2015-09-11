#ifndef KIP_XML_QNAME_HPP_
#define KIP_XML_QNAME_HPP_

#include <string>

#include "kip/hash-combine.hpp"


namespace kip {
namespace xml {

struct qname {
  std::string name;
  std::string url;

  qname() {}
  qname(std::string const& name, std::string const& url)
    : name(name)
    , url(url)
  {}

  bool empty() const {
    return name.empty() && url.empty();
  }

  size_t hash() const {
    size_t h = 0;
    hash_combine(h, name);
    hash_combine(h, url);
    return h;
  }
};

inline bool operator==(qname const& lhs, qname const& rhs) {
  return lhs.name == rhs.name &&
         lhs.url == rhs.url;
}

inline bool operator!=(qname const& lhs, qname const& rhs) {
  return !(lhs == rhs);
}

}  // namespace qname
}  // namespace pst


namespace std {

template <>
struct hash<kip::xml::qname> {
  using result_type = size_t;
  using argument_type = kip::xml::qname;

  size_t operator()(kip::xml::qname const& v) const {
    return v.hash();
  }
};

}  // namespace std


#endif  // KIP_XML_QNAME_HPP_
