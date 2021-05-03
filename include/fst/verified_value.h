#pragma once

#include <new>
#include <utility>
#include "fst/assert.h"

namespace fst {
template <typename _Tp>
class verified_value_base {};

template <typename _Tp, typename Enable = void>
class verified_value;

// template <typename _Tp, typename _Enum, _Enum _ValidId, typename Enable = void>
// class verified_value_e;

template <typename _Tp>
class verified_value<_Tp, typename std::enable_if<std::is_arithmetic_v<_Tp>>::type> : verified_value_base<_Tp> {
public:
  using value_type = _Tp;
  static_assert(!std::is_same_v<value_type, bool>, "verified_value cannot be a bool.");

  struct invalid_tag {};

  inline static verified_value invalid() noexcept { return verified_value(invalid_tag()); }

  verified_value() = delete;
  verified_value(const verified_value&) = delete;
  verified_value(verified_value&& vv) noexcept
      : _value(vv._value)
      , _is_valid(vv._is_valid) {
    vv._is_valid = false;
  }

  inline verified_value(invalid_tag) noexcept
      : _is_valid(false) {}

  inline constexpr verified_value(value_type value) noexcept
      : _value(value)
      , _is_valid(true) {}

  ~verified_value() noexcept = default;

  verified_value& operator=(const verified_value&) = delete;
  verified_value& operator=(verified_value&&) = delete;

  inline value_type& get() noexcept {
    fst_assert(is_valid(), "verified_value::get Value is invalid.");
    return _value;
  }

  inline const value_type& get() const noexcept {
    fst_assert(is_valid(), "verified_value::get Value is invalid.");
    return _value;
  }

  inline bool is_valid() const noexcept { return _is_valid; }
  inline explicit operator bool() const noexcept { return _is_valid; }
  inline bool operator==(bool b) const noexcept { return _is_valid == b; }
  inline bool operator!=(bool b) const noexcept { return _is_valid != b; }
  inline bool operator!() const noexcept { return !_is_valid; }

  inline operator value_type() const noexcept { return _value; }

private:
  value_type _value;
  bool _is_valid;
};

template <typename _Tp>
class verified_value<_Tp, typename std::enable_if<!std::is_arithmetic_v<_Tp>>::type> : verified_value_base<_Tp> {
public:
  using value_type = _Tp;

  struct invalid_tag {};

  inline static verified_value invalid() noexcept { return verified_value(invalid_tag()); }

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

  inline constexpr verified_value(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
      : _is_valid(true) {

    new (&_value) value_type(value);
  }

  inline constexpr verified_value(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
      : _is_valid(true) {
    new (&_value) value_type(std::move(value));
  }

  template <typename... Args>
  inline constexpr verified_value(Args&&... args) noexcept(std::is_nothrow_constructible_v<value_type, Args...>)
      : _is_valid(true) {
    new(&_value) value_type(std::forward<Args>(args)...);
  }

  inline ~verified_value() noexcept(std::is_nothrow_destructible_v<value_type>) {
    if(is_valid()) {
      get().~value_type();
    }
  }

  verified_value& operator=(const verified_value&) = delete;
  verified_value& operator=(verified_value&&) = delete;

  inline value_type& get() noexcept {
    fst_assert(is_valid(), "verified_value::get Value is invalid.");
    return *std::launder(reinterpret_cast<value_type*>(&_value));
  }

  inline const value_type& get() const noexcept {
    fst_assert(is_valid(), "verified_value::get Value is invalid.");
    return *std::launder(reinterpret_cast<const value_type*>(&_value));
  }

  inline bool is_valid() const noexcept { return _is_valid; }
  inline explicit operator bool() const noexcept { return _is_valid; }
  inline bool operator==(bool b) const noexcept { return _is_valid == b; }
  inline bool operator!=(bool b) const noexcept { return _is_valid != b; }
  inline bool operator!() const noexcept { return !_is_valid; }

  inline operator value_type() const noexcept(std::is_nothrow_copy_constructible_v<value_type>) { return get(); }

private:
  using aligned_type = typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type;
  aligned_type _value;
  bool _is_valid;
};
} // namespace fst.
