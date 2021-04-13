///
/// BSD 3-Clause License
///
/// Copyright (c) 2020, Alexandre Arsenault
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// * Redistributions of source code must retain the above copyright notice, this
///   list of conditions and the following disclaimer.
///
/// * Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
///
/// * Neither the name of the copyright holder nor the names of its
///   contributors may be used to endorse or promote products derived from
///   this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
///

#pragma once
#include "fst/assert.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <iosfwd>

namespace fst {
//
// not_null
//
// Restricts a pointer or smart pointer to only hold non-null values.
//
// Has zero size overhead over T.
//
// If T is a pointer (i.e. T == U*) then
// - allow construction from U*
// - disallow construction from nullptr_t
// - disallow default construction
// - ensure construction from null U* fails
// - allow implicit conversion to U*
//
template <class T>
class not_null {
public:
  static_assert(
      std::is_convertible<decltype(std::declval<T>() != nullptr), bool>::value, "T cannot be compared to nullptr.");

  template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
  constexpr not_null(U&& u)
      : _ptr(std::forward<U>(u)) {
    fst_assert(_ptr != nullptr, "Pointer is null.");
  }

  template <typename = std::enable_if_t<!std::is_same<std::nullptr_t, T>::value>>
  constexpr not_null(T u)
      : _ptr(std::move(u)) {
    fst_assert(_ptr != nullptr, "Pointer is null.");
  }

  template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
  constexpr not_null(const not_null<U>& other)
      : not_null(other.get()) {}

  not_null(const not_null& other) = default;
  not_null& operator=(const not_null& other) = default;
  constexpr std::conditional_t<std::is_copy_constructible<T>::value, T, const T&> get() const {
    fst_assert(_ptr != nullptr, "Pointer is null.");
    return _ptr;
  }

  constexpr operator T() const { return get(); }
  constexpr decltype(auto) operator-> () const { return get(); }
  constexpr decltype(auto) operator*() const { return *get(); }

  // prevents compilation when someone attempts to assign a null pointer constant
  not_null(std::nullptr_t) = delete;
  not_null& operator=(std::nullptr_t) = delete;

  // unwanted operators...pointers only point to single objects!
  not_null& operator++() = delete;
  not_null& operator--() = delete;
  not_null operator++(int) = delete;
  not_null operator--(int) = delete;
  not_null& operator+=(std::ptrdiff_t) = delete;
  not_null& operator-=(std::ptrdiff_t) = delete;
  void operator[](std::ptrdiff_t) const = delete;

private:
  T _ptr;
};

template <class T>
auto make_not_null(T&& t) noexcept {
  return not_null<std::remove_cv_t<std::remove_reference_t<T>>>{ std::forward<T>(t) };
}

template <class T>
std::ostream& operator<<(std::ostream& os, const not_null<T>& val) {
  os << val.get();
  return os;
}

template <class T, class U>
auto operator==(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() == rhs.get()))
    -> decltype(lhs.get() == rhs.get()) {
  return lhs.get() == rhs.get();
}

template <class T, class U>
auto operator!=(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() != rhs.get()))
    -> decltype(lhs.get() != rhs.get()) {
  return lhs.get() != rhs.get();
}

template <class T, class U>
auto operator<(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() < rhs.get()))
    -> decltype(lhs.get() < rhs.get()) {
  return lhs.get() < rhs.get();
}

template <class T, class U>
auto operator<=(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() <= rhs.get()))
    -> decltype(lhs.get() <= rhs.get()) {
  return lhs.get() <= rhs.get();
}

template <class T, class U>
auto operator>(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() > rhs.get()))
    -> decltype(lhs.get() > rhs.get()) {
  return lhs.get() > rhs.get();
}

template <class T, class U>
auto operator>=(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() >= rhs.get()))
    -> decltype(lhs.get() >= rhs.get()) {
  return lhs.get() >= rhs.get();
}

// more unwanted operators
template <class T, class U>
std::ptrdiff_t operator-(const not_null<T>&, const not_null<U>&) = delete;
template <class T>
not_null<T> operator-(const not_null<T>&, std::ptrdiff_t) = delete;
template <class T>
not_null<T> operator+(const not_null<T>&, std::ptrdiff_t) = delete;
template <class T>
not_null<T> operator+(std::ptrdiff_t, const not_null<T>&) = delete;

template <typename _Tp>
class optional_owned_ptr {
public:
  using element_type = std::remove_cvref_t<_Tp>;
  using pointer = element_type*;
  using const_pointer = const element_type*;

  optional_owned_ptr() noexcept
      : _ptr(nullptr)
      , _is_owned(false) {}

  optional_owned_ptr(std::nullptr_t) noexcept
      : optional_owned_ptr() {}

  optional_owned_ptr(pointer ptr, bool owned = true)
      : _ptr(ptr)
      , _is_owned(owned) {}

  optional_owned_ptr(std::nullptr_t, bool owned) noexcept
      : optional_owned_ptr() {}

  optional_owned_ptr(const optional_owned_ptr& oop) {
    fst_assert(!oop.is_owned(), "Can't copy an owned pointer.");
    _ptr = oop._ptr;
    _is_owned = false;
  }

  optional_owned_ptr(optional_owned_ptr&& oop)
      : _ptr(oop._ptr)
      , _is_owned(oop._is_owned) {
    oop._ptr = nullptr;
    oop._is_owned = false;
  }

  ~optional_owned_ptr() { reset(); }

  optional_owned_ptr& operator=(const optional_owned_ptr& oop) {
    fst_assert(!oop.is_owned(), "Can't copy an owned pointer.");

    reset();
    _ptr = oop._ptr;
    _is_owned = false;
    return *this;
  }

  optional_owned_ptr& operator=(optional_owned_ptr&& oop) {
    bool owned = oop.is_owned();
    pointer ptr = oop.release();
    reset(ptr, owned);
    return *this;
  }

  optional_owned_ptr& operator=(pointer ptr) {
    reset();
    _ptr = ptr;
    _is_owned = _ptr != nullptr;
    return *this;
  }

  optional_owned_ptr& operator=(std::nullptr_t) {
    reset();
    return *this;
  }

  inline bool is_valid() const { return _ptr != nullptr; }

  inline bool is_owned() const { return _is_owned; }

  inline pointer get() const noexcept { return _ptr; }

  typename std::add_lvalue_reference<element_type>::type operator*() const { return *_ptr; }

  inline pointer operator->() const noexcept { return _ptr; }

  inline explicit operator bool() const noexcept { return is_valid(); }

  inline void reset() {
    if (is_valid() && is_owned()) {
      delete _ptr;
    }

    _ptr = nullptr;
    _is_owned = false;
  }

  inline void reset(pointer ptr, bool owned) {
    reset();
    _ptr = ptr;
    _is_owned = _ptr == nullptr ? false : owned;
  }

  inline void reset(std::nullptr_t ptr, bool owned) {
    reset();

    _ptr = ptr;
    _is_owned = false;
  }

  /// Releases the ownership of the managed object if any.
  inline pointer release(bool keed_unowned = false) {
    pointer ptr = _ptr;
    if (!keed_unowned) {
      _ptr = nullptr;
    }
    _is_owned = false;
    return ptr;
  }

private:
  pointer _ptr;
  bool _is_owned;
};
} // namespace fst.
