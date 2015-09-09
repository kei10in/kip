#ifndef KIP_ELEMENTS_HPP_
#define KIP_ELEMENTS_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include "kip/xml-ns.hpp"


namespace kip {


template <class T, class U>
class named_element_map {
  using container_t = std::unordered_map<xml::qname, std::shared_ptr<U>>;
  container_t container_;

public:
  named_element_map() = default;

  bool operator==(named_element_map<T, U> const& rhs) const {
    return container_ == rhs.container_;
  }

  T find(xml::qname const& name) const {
    auto it = container_.find(name);
    if (it == container_.end()) {
      return T();
    } else {
      return T(*it);
    }
  }

  std::pair<typename container_t::iterator, bool>
    insert(std::pair<xml::qname, std::shared_ptr<U>> const& v) {
    return container_.insert(v);
  }

  size_t size() const {
    return container_.size();
  }

  template <class F>
  void traverse(F& f) const {
    for (auto const& it : container_) {
      T(it).traverse(f);
    }
  }

};


struct property_impl;
struct scored_property_impl;
struct option_impl;
struct feature_impl;
struct parameter_def_impl;

class property;
class scored_property;

class option;
class feature_def;

class parameter_def;

using property_collection = named_element_map<property, property_impl>;

using scored_property_collection = named_element_map<scored_property, scored_property_impl>;

// todo: rename to feature_def_collection
using feature_collection = named_element_map<feature_def, feature_impl>;

using parameter_def_collection = named_element_map<parameter_def, parameter_def_impl>;


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

  template <class F>
  void traverse(F& f) const {
    f.start(*this);
    f.end(*this);
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


struct property_impl {
  boost::optional<variable> value;
  property_collection properties;
};



inline bool operator==(property_impl const& lhs, property_impl const& rhs) {
  return lhs.value == rhs.value && lhs.properties == rhs.properties;
}

inline bool operator!=(property_impl const& lhs, property_impl const& rhs) {
  return !(lhs == rhs);
}



class property {
  xml::qname name_;
  std::shared_ptr<property_impl> pimpl;

public:
  property()
  {}

  property(std::pair<xml::qname, std::shared_ptr<property_impl>> const& v)
    : name_(v.first)
    , pimpl(v.second)
  {}

  property(xml::qname const& n, std::shared_ptr<property_impl> p)
    : name_(n)
    , pimpl(p)
  {}

  xml::qname const& name() const {
    return name_;
  }

  bool empty() {
    return pimpl == nullptr;
  }

  xml::qname& operator=(xml::qname const& v) {
    if (pimpl->value) {
      pimpl->value->data = v;
    } else {
      pimpl->value = variable(v);
    }
    return boost::get<xml::qname>(pimpl->value->data);
  }

  std::string& operator=(std::string const& v) {
    if (pimpl->value) {
      pimpl->value->data = v;
    } else {
      pimpl->value = variable(v);
    }
    return boost::get<std::string>(pimpl->value->data);
  }

  template <class T>
  T const& as() const {
    return boost::get<T>(pimpl->value->data);
  }

  template <class F>
  void traverse(F& f) const {
    f.start(*this);

    if (pimpl->value) {
      pimpl->value->traverse(f);
    }

    pimpl->properties.traverse(f);

    f.end(*this);
  }

};


struct scored_property_impl {
  boost::variant<boost::blank, variable, parameter_ref> content;
  boost::optional<variable> value;
  scored_property_collection scored_properties;
  property_collection properties;
};

inline bool operator==(scored_property_impl const& lhs, scored_property_impl const& rhs) {
  return lhs.value == rhs.value &&
    lhs.scored_properties == rhs.scored_properties &&
    lhs.properties == rhs.properties;
}

inline bool operator!=(scored_property_impl const& lhs, scored_property_impl const& rhs) {
  return !(lhs == rhs);
}


class scored_property {
  xml::qname name_;
  std::shared_ptr<scored_property_impl> pimpl;

public:
  scored_property() {}

  scored_property(std::pair<xml::qname, std::shared_ptr<scored_property_impl>> const& v)
    : name_(v.first)
    , pimpl(v.second)
  {}

  scored_property(xml::qname n, std::shared_ptr<scored_property_impl> p)
    : name_(n)
    , pimpl(p)
  {}

  bool empty() const {
    return pimpl == nullptr;
  }

  xml::qname const& name() const {
    return name_;
  }

  template <class T>
  T const& as() const {
    return boost::get<T>(pimpl->value->data);
  }

  template <class F>
  void traverse(F& f) const {
    f.start(*this);

    if (pimpl->value) {
      pimpl->value->traverse(f);
    }

    pimpl->properties.traverse(f);
    pimpl->scored_properties.traverse(f);

    f.end(*this);
  }
};



struct option_impl {
  boost::optional<xml::qname> const name;
  boost::optional<xml::qname> const constrained;
  scored_property_collection scored_properties;
  property_collection properties;

  option_impl(boost::optional<xml::qname> const& name) : name(name) {}
  option_impl(boost::optional<xml::qname> const& name,
    boost::optional<xml::qname> const& constrained)
    : name(name)
    , constrained(constrained)
  {}
};

inline bool operator==(option_impl const& lhs, option_impl const& rhs) {
  return lhs.name == rhs.name &&
    lhs.constrained == rhs.constrained &&
    lhs.scored_properties == rhs.scored_properties &&
    lhs.properties == rhs.properties;
}

inline bool operator!=(option_impl const& lhs, option_impl const& rhs) {
  return !(lhs == rhs);
}


class option {
  std::shared_ptr<option_impl> pimpl;

public:
  option()
  {}

  option(std::shared_ptr<option_impl> op)
    : pimpl(op)
  {}

  bool operator==(option const& rhs) const {
    return *pimpl == *rhs.pimpl;
  }

  bool empty() const {
    return pimpl == nullptr;
  }

  boost::optional<xml::qname> const& name() const {
    return pimpl->name;
  }

  boost::optional<xml::qname> const& constrained() const {
    return pimpl->constrained;
  }

  property property(xml::qname const& n) {
    return pimpl->properties.find(n);
  }

  scored_property scored_property(xml::qname const& n) {
    return pimpl->scored_properties.find(n);
  }

  // TODO: delete
  property_collection const& properties() const {
    return pimpl->properties;
  }

  // TODO: delete
  scored_property_collection const& scored_properties() const {
    return pimpl->scored_properties;
  }

  template <class F>
  void traverse(F& f) const {
    f.start(*this);

    pimpl->properties.traverse(f);
    pimpl->scored_properties.traverse(f);

    f.end(*this);
  }
};


struct feature_impl {
  std::vector<option> options;
  property_collection properties;
  feature_collection sub_features;
};

inline bool operator==(feature_impl const& lhs, feature_impl const& rhs) {
  return lhs.options == rhs.options &&
    lhs.properties == rhs.properties &&
    lhs.sub_features == rhs.sub_features;
}

inline bool operator!=(feature_impl const& lhs, feature_impl const& rhs) {
  return !(lhs == rhs);
}


class feature_def {
  xml::qname name_;
  std::shared_ptr<feature_impl> pimpl;

public:
  feature_def() {}

  feature_def(std::pair<xml::qname, std::shared_ptr<feature_impl>> const& ft)
    : name_(ft.first)
    , pimpl(ft.second)
  {}

  feature_def(xml::qname const& n, std::shared_ptr<feature_impl> pimpl)
    : name_(n)
    , pimpl(pimpl)
  {}

  bool empty() const { return pimpl == nullptr; }

  xml::qname const& name() const { return name_; }

  std::vector<option> const& options() const {
    return pimpl->options;
  }

  property property(xml::qname const& n) {
    return pimpl->properties.find(n);
  }

  feature_def sub_feature(xml::qname const& n) {
    return pimpl->sub_features.find(n);
  }

  // TODO: delete
  property_collection const& properties() const {
    return pimpl->properties;
  }

  // TODO: delete
  feature_collection const& sub_features() const {
    return pimpl->sub_features;
  }

  template <class F>
  void traverse(F& f) const {
    f.start(*this);

    for (auto const& o : pimpl->options) {
      o.traverse(f);
    }

    pimpl->properties.traverse(f);

    f.end(*this);
  }
};



struct parameter_init {
  boost::optional<variable> value;
};

inline bool operator==(parameter_init const& lhs, parameter_init const& rhs) {
  return lhs.value == rhs.value;
}

inline bool operator!=(parameter_init const& lhs, parameter_init const& rhs) {
  return !(lhs == rhs);
}


struct parameter_def_impl {
  property_collection properties;
};

inline bool operator==(parameter_def_impl const& lhs, parameter_def_impl const& rhs) {
  return lhs.properties == rhs.properties;
}

inline bool operator!=(parameter_def_impl const& lhs, parameter_def_impl const& rhs) {
  return !(lhs == rhs);
}


class parameter_def {
  xml::qname name_;
  std::shared_ptr<parameter_def_impl> pimpl;

public:
  parameter_def()
  {}

  parameter_def(std::pair<xml::qname, std::shared_ptr<parameter_def_impl>> pd)
    : name_(pd.first)
    , pimpl(pd.second)
  {}

  parameter_def(xml::qname const& n, std::shared_ptr<parameter_def_impl> p)
    : name_(n)
    , pimpl(p)
  {}

  bool empty() const {
    return pimpl == nullptr;
  }

  xml::qname const& name() const { return name_; }

  property property(xml::qname const& n) {
    return pimpl->properties.find(n);
  }

  template <class F>
  void traverse(F& f) const {
    f.start(*this);

    pimpl->properties.traverse(f);

    f.end(*this);
  }
};



struct print_capabilities_impl {
  int version;
  std::string device_namespace;
  std::string device_namespace_prefix;
  feature_collection features;
  property_collection properties;
  parameter_def_collection parameters;

  print_capabilities_impl(
    int v,
    std::string const& dn,
    std::string const& pref
    ) : version(v)
    , device_namespace(dn)
    , device_namespace_prefix(pref)
  {}
};

inline bool operator==(print_capabilities_impl const& lhs,
  print_capabilities_impl const& rhs) {
  return lhs.version == rhs.version &&
    lhs.device_namespace == rhs.device_namespace &&
    lhs.features == rhs.features &&
    lhs.properties == rhs.properties &&
    lhs.parameters == rhs.parameters;
}

inline bool operator!=(print_capabilities_impl const& lhs,
  print_capabilities_impl const& rhs) {
  return !(lhs == rhs);
}


class print_capabilities {
  bool empty_;
  std::shared_ptr<print_capabilities_impl> pimpl;

public:
  print_capabilities()
    : empty_(true)
    , pimpl(std::shared_ptr<print_capabilities_impl>())
  {}

  print_capabilities(std::shared_ptr<print_capabilities_impl> pimpl)
    : empty_(false)
    , pimpl(pimpl)
  {}

  bool empty() const { return empty_; }

  int version() const { return pimpl->version; }

  std::string const& device_namespace() const {
    return pimpl->device_namespace;
  }

  std::string const& device_namespace_prefix() const {
    return pimpl->device_namespace_prefix;
  }

  feature_def feature(xml::qname const& n) {
    return pimpl->features.find(n);
  }

  parameter_def parameter(xml::qname const& n) {
    return pimpl->parameters.find(n);
  }

  feature_collection const& features() const {
    return pimpl->features;
  }

  property_collection const& properties() const {
    return pimpl->properties;
  }

  parameter_def_collection const& parameters() const {
    return pimpl->parameters;
  }

  template <class F>
  void traverse(F& f) const {
    f.start(*this);

    pimpl->features.traverse(f);
    pimpl->parameters.traverse(f);
    pimpl->properties.traverse(f);

    f.end(*this);
  }

};



struct print_ticket_document {
  int version;
  feature_collection features;
  std::unordered_map<xml::qname, property_impl> properties;
  std::unordered_map<xml::qname, parameter_init> parameters;
};


}  // namespace kip


#endif  // KIP_ELEMENTS_HPP_
