#ifndef KIP_CORE_PSOM_ELEMENTS_HPP_
#define KIP_CORE_PSOM_ELEMENTS_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include "kip/xml-ns.hpp"


namespace kip {


enum class value_type { string, qname, integer, decimal, };


struct variable {
  value_type type;
  boost::variant<std::string, xml::qname, int, float> data;

  variable() : type(value_type::string), data() {}

  variable(std::string const& v)
    : type(value_type::string), data(v) {}

  variable(xml::qname const& v)
    : type(value_type::qname), data(v) {}

  variable(int v)
    : type(value_type::integer), data(v) {}

  variable(float v)
    : type(value_type::decimal), data(v) {}

  bool operator==(variable const& rhs) const {
    if (type != rhs.type) return false;
    return data == rhs.data;
  }

  bool operator==(int rhs) const {
    if (type != value_type::integer) return false;
    return boost::get<int>(data) == rhs;
  }

  bool operator==(char const* rhs) const {
    if (type != value_type::string) return false;
    return boost::get<std::string>(data) == rhs;
  }

  bool operator==(std::string const& rhs) const {
    if (type != value_type::string) return false;
    return boost::get<std::string>(data) == rhs;
  }

  bool operator==(xml::qname const& rhs) const {
    if (type != value_type::qname) return false;
    return boost::get<xml::qname>(data) == rhs;
  }
};


struct parameter_ref {
  xml::qname name;
};

inline bool operator==(parameter_ref const& lhs, parameter_ref const& rhs) {
  return lhs.name == rhs.name;
}

inline bool operator!=(parameter_ref const& lhs, parameter_ref const& rhs) {
  return !(lhs == rhs);
}


struct property {
  boost::optional<variable> value;
  std::unordered_map<xml::qname, property> properties;
};

inline bool operator==(property const& lhs, property const& rhs) {
  return lhs.value == rhs.value && lhs.properties == rhs.properties;
}

inline bool operator!=(property const& lhs, property const& rhs) {
  return !(lhs == rhs);
}


struct scored_property {
  boost::variant<boost::blank, variable, parameter_ref> content;
  boost::optional<variable> value;
  std::unordered_map<xml::qname, scored_property> scored_properties;
  std::unordered_map<xml::qname, property> properties;
};

inline bool operator==(scored_property const& lhs, scored_property const& rhs) {
  return lhs.value == rhs.value &&
         lhs.scored_properties == rhs.scored_properties &&
         lhs.properties == rhs.properties;
}

inline bool operator!=(scored_property const& lhs, scored_property const& rhs) {
  return !(lhs == rhs);
}



struct option {
  boost::optional<xml::qname> const name;
  boost::optional<xml::qname> const constrained;
  std::unordered_map<xml::qname, scored_property> scored_properties;
  std::unordered_map<xml::qname, property> properties;

  option(boost::optional<xml::qname> const& name) : name(name) {}
  option(boost::optional<xml::qname> const& name,
         boost::optional<xml::qname> const& constrained)
    : name(name)
    , constrained(constrained)
  {}
};

inline bool operator==(option const& lhs, option const& rhs) {
  return lhs.name == rhs.name &&
         lhs.constrained == rhs.constrained &&
         lhs.scored_properties == rhs.scored_properties &&
         lhs.properties == rhs.properties;
}

inline bool operator!=(option const& lhs, option const& rhs) {
  return !(lhs == rhs);
}


struct feature {
  std::vector<option> options;
  std::unordered_map<xml::qname, property> properties;
  std::unordered_map<xml::qname, feature> sub_features;
};

inline bool operator==(feature const& lhs, feature const& rhs) {
  return lhs.options == rhs.options &&
         lhs.properties == rhs.properties &&
         lhs.sub_features == rhs.sub_features;
}

inline bool operator!=(feature const& lhs, feature const& rhs) {
  return !(lhs == rhs);
}


struct parameter_init {
  boost::optional<variable> value;
};

inline bool operator==(parameter_init const& lhs, parameter_init const& rhs) {
  return lhs.value == rhs.value;
}

inline bool operator!=(parameter_init const& lhs, parameter_init const& rhs) {
  return !(lhs == rhs);
}


struct parameter_def {
  std::unordered_map<xml::qname, property> properties;
};

inline bool operator==(parameter_def const& lhs, parameter_def const& rhs) {
  return lhs.properties == rhs.properties;
}

inline bool operator!=(parameter_def const& lhs, parameter_def const& rhs) {
  return !(lhs == rhs);
}


struct print_schema_document {
  int version;
  std::unordered_map<xml::qname, feature> features;
  std::unordered_map<xml::qname, property> properties;
};

struct print_capabilities_document {
  int version;
  std::string device_namespace;
  std::string device_namespace_prefix;
  std::unordered_map<xml::qname, feature> features;
  std::unordered_map<xml::qname, property> properties;
  std::unordered_map<xml::qname, parameter_def> parameters;
};

inline bool operator==(print_capabilities_document const& lhs,
                       print_capabilities_document const& rhs) {
  return lhs.version == rhs.version &&
         lhs.device_namespace == rhs.device_namespace &&
         lhs.features == rhs.features &&
         lhs.properties == rhs.properties &&
         lhs.parameters == rhs.parameters;
}

inline bool operator!=(print_capabilities_document const& lhs,
                       print_capabilities_document const& rhs) {
  return !(lhs == rhs);
}


struct print_ticket_document {
  int version;
  std::unordered_map<xml::qname, feature> features;
  std::unordered_map<xml::qname, property> properties;
  std::unordered_map<xml::qname, parameter_init> parameters;
};


}  // namespace kip


#endif  // KIP_CORE_DOM_ELEMENTS_HPP_
