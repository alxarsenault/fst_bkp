#pragma once

#include <utility>
#include "fst/assert.h"

namespace fst {
template <typename _Tp>
class verified_value {
public:
  using value_type = _Tp;
  struct invalid_tag {};

  inline static verified_value invalid() {
    return verified_value(invalid_tag());
  }

  verified_value() = delete;
  verified_value(const verified_value&) = delete;
  verified_value(verified_value&& vv)
      : _is_valid(vv._is_valid) {
    if (_is_valid) {
      new (&_value) value_type(std::move(get()));
      vv._is_valid = false;
    }
  }

  inline verified_value(invalid_tag) noexcept : _is_valid(false) {}

  inline constexpr verified_value(const value_type& value)
      : _is_valid(true) {
  
      new(&_value) value_type(value);
      
      }

  inline constexpr verified_value(value_type&& value)
      : _is_valid(true) {
      new(&_value) value_type(std::move(value));
      
      }
      
  template<typename ... Args>
  inline constexpr verified_value(Args&&... args)
  : _is_valid(true)
  {
    new(&_value) value_type(std::forward<Args>(args)...);
  }
  
  ~verified_value() {
    if(is_valid()) {
      get().~value_type();
    }
  }
  
  verified_value& operator=(const verified_value&) = delete;
  verified_value& operator=(verified_value&&) = delete;

  inline value_type& get() {
    fst_assert(is_valid(), "verified_value::get Value is invalid.");
    return *reinterpret_cast<value_type*>(&_value);
  }

  inline const value_type& get() const {
  fst_assert(is_valid(), "verified_value::get Value is invalid.");
  return *reinterpret_cast<const value_type*>(&_value); }

  inline bool is_valid() const { return _is_valid; }
  inline explicit operator bool() const { return _is_valid; }
  inline bool operator==(bool b) const { return _is_valid == b; }
  inline bool operator!=(bool b) const { return _is_valid != b; }
  inline bool operator!() const { return !_is_valid; }

  inline operator value_type() const { return get(); }

private:
  using aligned_type = typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type;
  aligned_type _value;
  bool _is_valid;
  
  
};
} // namespace fst.
