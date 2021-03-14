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
#include "fst/traits.h"
#include "fst/util.h"
#include "fst/verified_value.h"
#include <algorithm>
#include <array>
#include <cstring>
#include <cstdio>
#include <iterator>
#include <iostream>
#include <initializer_list>
#include <limits>
#include <string>
#include <string_view>
#include <sstream>
#include <stdexcept>

namespace fst {
template <typename _CharT, std::size_t _Size>
class basic_small_string {
public:
  using __self = basic_small_string;
  using view_type = std::basic_string_view<_CharT>;
  using string_type = std::basic_string<_CharT>;
  using traits_type = std::char_traits<_CharT>;
  using value_type = _CharT;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  static constexpr size_type maximum_size = _Size;
  static constexpr size_type npos = std::numeric_limits<size_type>::max();

  static_assert(!std::is_array<value_type>::value, "Character type of basic_small_string must not be an array.");
  static_assert(
      std::is_standard_layout<value_type>::value, "Character type of basic_small_string must be standard-layout.");
  static_assert(std::is_trivial<value_type>::value, "Character type of basic_small_string must be trivial.");
  static_assert(std::is_same<value_type, typename traits_type::char_type>::value,
      "traits_type::char_type must be the same type as value_type.");

  constexpr basic_small_string() noexcept = default;

  inline constexpr basic_small_string(size_type count, value_type ch) noexcept {
    fst_assert(count <= maximum_size, "basic_small_string count must be smaller or equal to maximum_size.");
    std::fill_n(_data.data(), count, ch);
    _size = count;
    _data[_size] = 0;
  }

  inline constexpr basic_small_string(const basic_small_string& other) noexcept
      : _data(other._data)
      , _size(other._size) {}

  inline constexpr basic_small_string(basic_small_string&& other) noexcept
      : _data(std::move(other._data))
      , _size(other._size) {

    other._size = 0;
    other._data[0] = 0;
  }

  inline constexpr basic_small_string(const basic_small_string& other, size_type pos, size_type count = npos) noexcept {
    fst_assert(pos <= other._size, "basic_small_string pos must be smaller or equal to size.");
    _size = fst::minimum(count, other._size - pos);
    std::copy_n(other._data.data() + pos, _size, _data.data());
    _data[_size] = 0;
  }

  inline constexpr basic_small_string(const value_type* s) noexcept {
    _size = c_strlen(s);
    fst_assert(_size <= maximum_size, "basic_small_string c string must be smaller or equal to maximum_size.");
    std::copy_n(s, _size, _data.data());
    _data[_size] = 0;
  }

  inline constexpr basic_small_string(const value_type* s, size_type count) noexcept {
    fst_assert(count <= maximum_size, "basic_small_string count must be smaller or equal to maximum_size.");
    fst_assert(count <= c_strlen(s), "basic_small_string count must be smaller or equal to c string size.");
    _size = count;
    std::copy_n(s, _size, _data.data());
    _data[_size] = 0;
  }

  template <class InputIt>
  inline constexpr basic_small_string(InputIt first, InputIt last) noexcept {
    _size = std::distance(first, last);
    fst_assert(
        _size <= maximum_size, "basic_small_string iteration distance must be smaller or equal to maximum_size.");
    std::copy_n(first, last, _data.data());
    _data[_size] = 0;
  }

  inline constexpr basic_small_string(std::initializer_list<value_type> ilist) noexcept {
    fst_assert(ilist.size() <= maximum_size,
        "basic_small_string initializer_list size must be smaller or equal to maximum_size.");
    _size = ilist.size();
    std::copy_n(ilist.begin(), ilist.end(), _data.data());
    _data[_size] = 0;
  }

  inline constexpr basic_small_string(view_type v) noexcept {
    fst_assert(v.size() <= maximum_size, "basic_small_string view size must be smaller or equal to maximum_size.");
    _size = v.size();
    std::copy_n(v.begin(), v.end(), _data.data());
    _data[_size] = 0;
  }

  inline constexpr basic_small_string(view_type v, size_type pos, size_type count = npos) noexcept {
    fst_assert(pos <= v.size(), "basic_small_string pos must be smaller or equal to view size.");
    _size = fst::minimum(count, v.size() - pos);
    fst_assert(_size <= maximum_size, "basic_small_string size must be smaller or equal to maximum_size.");
    std::copy_n(v.data() + pos, _size, _data.data());
    _data[_size] = 0;
  }

  inline constexpr basic_small_string(const string_type& s) noexcept
      : basic_small_string(view_type(s)) {}

  inline constexpr basic_small_string(const string_type& s, size_type pos, size_type count = npos) noexcept
      : basic_small_string(view_type(s), pos, count) {}

  template <class T,
      class =
          typename std::enable_if<fst::is_convertible_to_string_view<value_type, traits_type, T>::value, void>::type>
  inline explicit constexpr basic_small_string(const T& t) noexcept
      : basic_small_string(view_type(t)) {}

  template <class T,
      class =
          typename std::enable_if<fst::is_convertible_to_string_view<value_type, traits_type, T>::value, void>::type>
  inline explicit constexpr basic_small_string(const T& t, size_type pos, size_type count = npos) noexcept
      : basic_small_string(view_type(t), pos, count) {}

  inline constexpr basic_small_string& operator=(const basic_small_string& other) noexcept {
    _data = other._data;
    _size = other._size;
    return *this;
  }

  inline constexpr basic_small_string& operator=(basic_small_string&& other) noexcept {
    _data = std::move(other._data);
    _size = other._size;
    other._size = 0;
    return *this;
  }

  inline constexpr basic_small_string& operator=(view_type v) noexcept {
    fst_assert(v.size() <= maximum_size, "basic_small_string view size must be smaller or equal to maximum_size.");
    _size = v.size();
    std::copy_n(v.begin(), v.end(), _data.data());
    _data[_size] = 0;
    return *this;
  }

  inline constexpr basic_small_string& operator=(const string_type& s) noexcept { return operator=(view_type(s)); }

  inline constexpr basic_small_string& operator=(std::initializer_list<value_type> ilist) noexcept {
    fst_assert(ilist.size() <= maximum_size,
        "basic_small_string initializer_list size must be smaller or equal to maximum_size.");
    _size = ilist.size();
    std::copy_n(ilist.begin(), ilist.end(), _data.data());
    _data[_size] = 0;
    return *this;
  }

  inline constexpr basic_small_string& operator=(const value_type* s) noexcept {
    _size = c_strlen(s);
    fst_assert(_size <= maximum_size, "basic_small_string c string must be smaller or equal to maximum_size.");
    std::copy_n(s, _size, _data.data());
    _data[_size] = 0;
    return *this;
  }

  template <class T,
      class =
          typename std::enable_if<fst::is_convertible_to_string_view<value_type, traits_type, T>::value, void>::type>
  inline constexpr basic_small_string& operator=(const T& t) noexcept {
    return operator=(view_type(t));
  }

  // Iterators.
  inline constexpr iterator begin() noexcept { return iterator(_data.data()); }
  inline constexpr const_iterator begin() const noexcept { return const_iterator(_data.data()); }

  inline constexpr iterator end() noexcept { return iterator(_data.data() + _size); }
  inline constexpr const_iterator end() const noexcept { return const_iterator(_data.data() + _size); }

  inline constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  inline constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

  inline constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  inline constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

  inline constexpr const_iterator cbegin() const noexcept { return begin(); }
  inline constexpr const_iterator cend() const noexcept { return end(); }

  inline constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
  inline constexpr const_reverse_iterator crend() const noexcept { return rend(); }

  // Capacity.
  [[nodiscard]] inline constexpr size_type size() const noexcept { return _size; }
  [[nodiscard]] inline constexpr size_type length() const noexcept { return _size; }
  [[nodiscard]] inline constexpr size_type max_size() const noexcept { return maximum_size; }
  [[nodiscard]] inline constexpr size_type capacity() const noexcept { return maximum_size; }
  [[nodiscard]] inline constexpr bool empty() const noexcept { return _size == 0; }

  // Element access.
  inline constexpr reference at(size_type pos) {
    if (pos >= size()) {
      throw std::out_of_range("basic_small_string::at");
    }

    return _data[pos];
  }
  inline constexpr const_reference at(size_type pos) const {
    if (pos >= size()) {
      throw std::out_of_range("basic_small_string::at");
    }

    return _data[pos];
  }

  inline constexpr reference operator[](size_type n) noexcept {
    fst_assert(n < maximum_size, "Index out of bounds");
    return _data[n];
  }
  inline constexpr const_reference operator[](size_type n) const noexcept {
    fst_assert(n < maximum_size, "Index out of bounds");
    return _data[n];
  }

  inline constexpr reference front() noexcept {
    fst_assert(_size > 0, "basic_small_string::front when empty.");
    return _data[0];
  }
  inline constexpr const_reference front() const noexcept {
    fst_assert(_size > 0, "basic_small_string::front when empty.");
    return _data[0];
  }

  inline constexpr reference back() noexcept {
    fst_assert(_size > 0, "basic_small_string::back when empty.");
    return _data[_size - 1];
  }

  inline constexpr const_reference back() const noexcept {
    fst_assert(_size > 0, "basic_small_string::back when empty.");
    return _data[_size - 1];
  }

  inline constexpr pointer data() noexcept { return _data.data(); }
  inline constexpr const_pointer data() const noexcept { return _data.data(); }

  //
  // Operations.
  //

  inline constexpr void clear() noexcept {
    _data[0] = 0;
    _size = 0;
  }

  inline constexpr void push_back(value_type c) noexcept {
    fst_assert(
        _size + 1 <= maximum_size, "basic_small_string::push_back size would end up greather than maximum_size.");
    _data[++_size] = c;
    _data[_size] = 0;
  }

  inline constexpr void pop_back() noexcept {
    fst_assert(_size, "basic_small_string::pop_back when empty.");
    _size--;
    _data[_size] = 0;
  }

  inline constexpr basic_small_string& append(size_type count, value_type c) noexcept {
    fst_assert(
        _size + count <= maximum_size, "basic_small_string::append size would end up greather than maximum_size.");
    std::fill_n(_data.data() + _size, count, c);
    _size += count;
    _data[_size] = 0;
    return *this;
  }

  inline constexpr basic_small_string& append(const basic_small_string& other) noexcept {
    fst_assert(_size + other.size() <= maximum_size,
        "basic_small_string::append size would end up greather than maximum_size.");
    std::copy_n(other.data(), other.size(), _data.data() + _size);
    _size += other.size();
    _data[_size] = 0;
    return *this;
  }

  inline constexpr basic_small_string& append(
      const basic_small_string& other, size_type pos, size_type count = npos) noexcept {
    fst_assert(pos <= other.size(), "basic_small_string pos must be smaller or equal to string size.");
    size_type o_size = fst::minimum(count, other.size()) - pos;
    fst_assert(
        _size + o_size <= maximum_size, "basic_small_string::append size would end up greather than maximum_size.");

    std::copy_n(other.data() + pos, o_size, _data.data() + _size);
    _size += o_size;
    _data[_size] = 0;
    return *this;
  }

  inline constexpr basic_small_string& append(view_type v) noexcept {
    fst_assert(
        _size + v.size() <= maximum_size, "basic_small_string::append size would end up greather than maximum_size.");

    std::copy_n(v.data(), v.size(), _data.data() + _size);
    _size += v.size();
    _data[_size] = 0;
    return *this;
  }

  inline constexpr basic_small_string& append(view_type v, size_type pos, size_type count = npos) noexcept {
    fst_assert(pos <= v.size(), "basic_small_string pos must be smaller or equal to view size.");
    size_type o_size = fst::minimum(count, v.size()) - pos;
    fst_assert(
        _size + o_size <= maximum_size, "basic_small_string::append size would end up greather than maximum_size.");

    std::copy_n(v.data() + pos, o_size, _data.data() + _size);
    _size += o_size;
    _data[_size] = 0;
    return *this;
  }

  inline constexpr basic_small_string& append(const value_type* s) noexcept { return append(view_type(s)); }

  inline constexpr basic_small_string& append(const value_type* s, size_type pos, size_type count = npos) noexcept {
    return append(view_type(s), pos, count);
  }

  inline constexpr basic_small_string& append(const string_type& s) noexcept { return append(view_type(s)); }

  inline constexpr basic_small_string& append(const string_type& s, size_type pos, size_type count = npos) noexcept {
    return append(view_type(s), pos, count);
  }

  template <class InputIt>
  constexpr basic_small_string& append(InputIt first, InputIt last) noexcept {
    return append(view_type(first, last));
  }

  constexpr basic_small_string& append(std::initializer_list<value_type> ilist) noexcept {
    return append(view_type(ilist.begin(), ilist.end()));
  }

  template <class T,
      class =
          typename std::enable_if<fst::is_convertible_to_string_view<value_type, traits_type, T>::value, void>::type>
  inline constexpr basic_small_string& append(const T& t) noexcept {
    return append(view_type(t));
  }

  template <class T,
      class =
          typename std::enable_if<fst::is_convertible_to_string_view<value_type, traits_type, T>::value, void>::type>
  inline constexpr basic_small_string& append(const T& t, size_type pos, size_type count = npos) noexcept {
    return append(view_type(t), pos, count);
  }

  inline constexpr basic_small_string& operator+=(const basic_small_string& other) noexcept { return append(other); }

  inline constexpr basic_small_string& operator+=(view_type v) noexcept { return append(v); }

  inline constexpr basic_small_string& operator+=(const value_type* s) noexcept { return append(view_type(s)); }

  inline constexpr basic_small_string& operator+=(const string_type& s) noexcept { return append(view_type(s)); }

  constexpr basic_small_string& operator+=(std::initializer_list<value_type> ilist) noexcept {
    return append(view_type(ilist.begin(), ilist.end()));
  }

  template <class T,
      class =
          typename std::enable_if<fst::is_convertible_to_string_view<value_type, traits_type, T>::value, void>::type>
  inline constexpr basic_small_string& operator+=(const T& t) noexcept {
    return append(view_type(t));
  }

  //
  //

  //
  inline constexpr void resize(size_type count, value_type c = value_type()) {
    fst_assert(count <= maximum_size, "basic_small_string::resize count must be smaller or equal to maximum_size.");
    if (count < _size) {
      _size = count;
      _data[_size] = 0;
    }
    else if (count > _size) {
      std::fill_n(_data.data() + _size, count - _size, c);
      _size = count;
      _data[_size] = 0;
    }
  }

  //
  // Convert.
  //
  inline string_type to_string() const { return string_type(data(), size()); }
  inline operator string_type() const { return to_string(); }

  inline constexpr view_type to_view() const { return view_type(data(), size()); }
  inline constexpr operator view_type() const { return to_view(); }

  template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value, void>::type>
  inline fst::verified_value<std::remove_cvref_t<T>> to() const {
    using v_value = fst::verified_value<std::remove_cvref_t<T>>;
    std::remove_cvref_t<T> result;
    return std::sscanf(_data.data(), type_to_format<std::remove_cvref_t<T>>(), &result) == EOF ? v_value::invalid()
                                                                                               : v_value(result);
  }

  template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value, void>::type>
  inline bool from(T value) {
    int result
        = std::snprintf(_data.data(), maximum_size_with_escape_char, type_to_format<std::remove_cvref_t<T>>(), value);
    _size = fst::maximum(result, 0);
    return _size;
  }

  template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value, void>::type>
  inline static basic_small_string to_string(T value) {
    basic_small_string s;
    s.from(value);
    return s;
  }

  friend std::ostream& operator<<(std::ostream& stream, const basic_small_string& bss) {
    return stream << bss.to_view();
  }

private:
  static constexpr size_type maximum_size_with_escape_char = maximum_size + 1;
  std::array<value_type, maximum_size_with_escape_char> _data;
  std::size_t _size = 0;

  inline constexpr size_type c_strlen(const char* str) noexcept { return *str ? 1 + c_strlen(str + 1) : 0; }

  template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value, void>::type>
  inline const char* type_to_format() const {
    if constexpr (std::is_integral<T>::value) {
      if constexpr (std::is_same<T, std::size_t>::value) {
        return "%zu";
      }
      else if constexpr (std::is_same<T, std::ptrdiff_t>::value) {
        return "%td";
      }
      else if constexpr (std::is_same<T, char>::value) {
        return "%hhd";
      }
      else if constexpr (std::is_same<T, short>::value) {
        return "%hd";
      }
      else if constexpr (std::is_same<T, int>::value) {
        return "%d";
      }
      else if constexpr (std::is_same<T, long>::value) {
        return "%ld";
      }
      else if constexpr (std::is_same<T, long long>::value) {
        return "%lld";
      }
      else if constexpr (std::is_same<T, unsigned char>::value) {
        return "%hhu";
      }
      else if constexpr (std::is_same<T, unsigned short>::value) {
        return "%hu";
      }
      else if constexpr (std::is_same<T, unsigned int>::value) {
        return "%u";
      }
      else if constexpr (std::is_same<T, unsigned long>::value) {
        return "%lu";
      }
      else if constexpr (std::is_same<T, unsigned long long>::value) {
        return "%llu";
      }
      else {
        return "%d";
      }
    }
    else if constexpr (std::is_floating_point<T>::value) {
      if constexpr (std::is_same<T, float>::value) {
        return "%f";
      }
      else if constexpr (std::is_same<T, long double>::value) {
        return "%LF";
      }
      else {
        return "%lf";
      }
    }

    return "%d";
  }
};

//
// Operator ==
//
template <class _CharT, std::size_t _Size>
inline bool operator==(
    const basic_small_string<_CharT, _Size>& __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  using view_type = typename basic_small_string<_CharT, _Size>::view_type;
  return (__lhs.size() == __rhs.size()) && (view_type(__lhs) == view_type(__rhs));
}

template <class _CharT, std::size_t _Size>
inline bool operator==(const basic_small_string<_CharT, _Size>& __lhs, const _CharT* __rhs) noexcept {
  using view_type = typename basic_small_string<_CharT, _Size>::view_type;
  return view_type(__lhs) == view_type(__rhs);
}

template <class _CharT, std::size_t _Size>
inline bool operator==(const _CharT* __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  using view_type = typename basic_small_string<_CharT, _Size>::view_type;
  return view_type(__lhs) == view_type(__rhs);
}

//
// Operator !=
//
template <class _CharT, std::size_t _Size>
inline bool operator!=(
    const basic_small_string<_CharT, _Size>& __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  return !(__lhs == __rhs);
}

template <class _CharT, std::size_t _Size>
inline bool operator!=(const basic_small_string<_CharT, _Size>& __lhs, const _CharT* __rhs) noexcept {
  return !(__lhs == __rhs);
}

template <class _CharT, std::size_t _Size>
inline bool operator!=(const _CharT* __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  return !(__lhs == __rhs);
}

//
// Operator <
//
template <class _CharT, std::size_t _Size>
inline bool operator<(
    const basic_small_string<_CharT, _Size>& __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  using view_type = typename basic_small_string<_CharT, _Size>::view_type;
  return view_type(__lhs).compare(view_type(__rhs)) < 0;
}

template <class _CharT, std::size_t _Size>
inline bool operator<(const basic_small_string<_CharT, _Size>& __lhs, const _CharT* __rhs) noexcept {
  using view_type = typename basic_small_string<_CharT, _Size>::view_type;
  return view_type(__lhs).compare(view_type(__rhs)) < 0;
}

template <class _CharT, std::size_t _Size>
inline bool operator<(const _CharT* __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  using view_type = typename basic_small_string<_CharT, _Size>::view_type;
  return view_type(__lhs).compare(view_type(__rhs)) < 0;
}

//
// Operator >
//
template <class _CharT, std::size_t _Size>
inline bool operator>(
    const basic_small_string<_CharT, _Size>& __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  return __rhs < __lhs;
}

template <class _CharT, std::size_t _Size>
inline bool operator>(const basic_small_string<_CharT, _Size>& __lhs, const _CharT* __rhs) noexcept {
  return __rhs < __lhs;
}

template <class _CharT, std::size_t _Size>
inline bool operator>(const _CharT* __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  return __rhs < __lhs;
}

//
// Operator <=
//
template <class _CharT, std::size_t _Size>
inline bool operator<=(
    const basic_small_string<_CharT, _Size>& __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  return !(__rhs < __lhs);
}

template <class _CharT, std::size_t _Size>
inline bool operator<=(const basic_small_string<_CharT, _Size>& __lhs, const _CharT* __rhs) noexcept {
  return !(__rhs < __lhs);
}

template <class _CharT, std::size_t _Size>
inline bool operator<=(const _CharT* __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  return !(__rhs < __lhs);
}

//
// Operator >=
//
template <class _CharT, std::size_t _Size>
inline bool operator>=(
    const basic_small_string<_CharT, _Size>& __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  return !(__lhs < __rhs);
}

template <class _CharT, std::size_t _Size>
inline bool operator>=(const basic_small_string<_CharT, _Size>& __lhs, const _CharT* __rhs) noexcept {
  return !(__lhs < __rhs);
}

template <class _CharT, std::size_t _Size>
inline bool operator>=(const _CharT* __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  return !(__lhs < __rhs);
}

// operator +
template <class _CharT, std::size_t _Size>
inline basic_small_string<_CharT, _Size> operator+(
    const basic_small_string<_CharT, _Size>& __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  basic_small_string<_CharT, _Size> s = __lhs;
  s += __rhs;
  return s;
}

// template<class _CharT, class _Traits, class _Allocator>
// basic_string<_CharT, _Traits, _Allocator>
// operator+(_CharT __lhs, const basic_string<_CharT,_Traits,_Allocator>& __rhs)
//{
//    basic_string<_CharT, _Traits, _Allocator> __r(__rhs.get_allocator());
//    typename basic_string<_CharT, _Traits, _Allocator>::size_type __rhs_sz = __rhs.size();
//    __r.__init(&__lhs, 1, 1 + __rhs_sz);
//    __r.append(__rhs.data(), __rhs_sz);
//    return __r;
//}

template <class _CharT, std::size_t _Size>
inline basic_small_string<_CharT, _Size> operator+(
    const basic_small_string<_CharT, _Size>& __lhs, _CharT __rhs) noexcept {
  basic_small_string<_CharT, _Size> s = __lhs;
  s.push_back(__rhs);
  return s;
}

template <class _CharT, std::size_t _Size>
inline basic_small_string<_CharT, _Size> operator+(
    const _CharT* __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  basic_small_string<_CharT, _Size> s = __lhs;
  s += __rhs;
  return s;
}

template <class _CharT, std::size_t _Size>
inline basic_small_string<_CharT, _Size> operator+(
    const basic_small_string<_CharT, _Size>& __lhs, const _CharT* __rhs) noexcept {
  basic_small_string<_CharT, _Size> s = __lhs;
  s += __rhs;
  return s;
}

template <class _CharT, std::size_t _Size>
inline basic_small_string<_CharT, _Size> operator+(
    basic_small_string<_CharT, _Size>&& __lhs, const basic_small_string<_CharT, _Size>& __rhs) noexcept {
  return std::move(__lhs.append(__rhs));
}

// template <class _CharT, std::size_t _Size>
// inline basic_small_string<_CharT, _Size> operator+(const basic_small_string<_CharT, _Size>& __lhs,
// basic_small_string<_CharT, _Size>&& __rhs) noexcept{
//  return std::move(__rhs.insert(0, __lhs));
//}

template <class _CharT, std::size_t _Size>
inline basic_small_string<_CharT, _Size> operator+(
    basic_small_string<_CharT, _Size>&& __lhs, basic_small_string<_CharT, _Size>&& __rhs) noexcept {
  return std::move(__lhs.append(__rhs));
}

template <class _CharT, std::size_t _Size>
inline basic_small_string<_CharT, _Size> operator+(
    basic_small_string<_CharT, _Size>&& __lhs, const _CharT* __rhs) noexcept {
  return std::move(__lhs.append(__rhs));
}

// template <class _CharT, std::size_t _Size>
// inline basic_small_string<_CharT, _Size> operator+(const _CharT* __lhs, basic_small_string<_CharT, _Size>&& __rhs)
// noexcept{
//  return std::move(__rhs.insert(0, __lhs));
//}

// template <class _CharT, std::size_t _Size>
// inline basic_small_string<_CharT, _Size> operator+(_CharT __lhs, basic_small_string<_CharT, _Size>&& __rhs) noexcept{
//  __rhs.insert(__rhs.begin(), __lhs);
//  return std::move(__rhs);
//}

template <class _CharT, std::size_t _Size>
inline basic_small_string<_CharT, _Size> operator+(basic_small_string<_CharT, _Size>&& __lhs, _CharT __rhs) noexcept {
  __lhs.push_back(__rhs);
  return std::move(__lhs);
}

// template <typename _CharT, std::size_t _Size>
// class basic_small_string_allocator {
// public:
//  using value_type = _CharT;
//  using size_type = std::size_t;
//  using difference_type = std::ptrdiff_t;
//  using propagate_on_container_move_assignment = std::true_type;
//
//  template <class U>
//  struct rebind {
//    typedef basic_small_string_allocator<U, _Size> other;
//  };
//
//  basic_small_string_allocator() = default;
//
//  inline basic_small_string_allocator(const basic_small_string_allocator&)
//      : basic_small_string_allocator() {}
//
//  inline basic_small_string_allocator(basic_small_string_allocator&& m)
//      : _buffer(std::move(m._buffer))
//      , _index(m._index) {
//    m._index = 0;
//  }
//
//  basic_small_string_allocator& operator=(const basic_small_string_allocator&) = delete;
//
//  inline basic_small_string_allocator& operator=(basic_small_string_allocator&& m) {
//    _buffer = std::move(m._buffer);
//    _index = m._index;
//    m._index = 0;
//    return *this;
//  }
//
//  inline static constexpr std::size_t size() noexcept { return _Size; }
//
//  inline value_type* allocate(std::size_t n) {
//    std::size_t byte_size = n * sizeof(value_type);
//    fst_assert(_index + byte_size <= _Size, "Out of memory.");
//    value_type* ptr = _buffer.data() + _index;
//    _index += byte_size;
//    return ptr;
//  }
//
//  inline void deallocate(value_type*, std::size_t n) {
//    std::size_t byte_size = n * sizeof(value_type);
//    fst_assert(byte_size <= _index, "Deallocating too much memory.");
//    _index -= byte_size;
//  }
//
// private:
//  std::array<value_type, _Size> _buffer;
//  std::size_t _index = 0;
//};
//
// template <typename T, typename U, std::size_t _Size>
// inline bool operator==(const basic_small_string_allocator<T, _Size>&, const basic_small_string_allocator<U, _Size>&)
// {
//  return true;
//}
//
// template <typename T, typename U, std::size_t _Size>
// inline bool operator!=(const basic_small_string_allocator<T, _Size>&, const basic_small_string_allocator<U, _Size>&)
// {
//  return false;
//}

// namespace small_string_detail {
//  template <typename _Tp, std::size_t _Size>
//  class small_string_allocator {
//  public:
//    using value_type = _Tp;
//    using size_type = std::size_t;
//    using difference_type = std::ptrdiff_t;
//    using propagate_on_container_move_assignment = std::true_type;
//
//    template <class U>
//    struct rebind {
//      typedef small_string_allocator<U, _Size> other;
//    };
//
//    small_string_allocator() = default;
//
//    inline small_string_allocator(const small_string_allocator&)
//        : small_string_allocator() {}
//
//    inline small_string_allocator(small_string_allocator&& m)
//        : _buffer(std::move(m._buffer))
//        , _index(m._index) {
//      m._index = 0;
//    }
//
//    small_string_allocator& operator=(const small_string_allocator&) = delete;
//    small_string_allocator& operator=(small_string_allocator&& m) {
//      _buffer = std::move(m._buffer);
//      _index = m._index;
//      m._index = 0;
//      return *this;
//    }
//
//    inline static constexpr std::size_t size() noexcept { return _Size; }
//
//    inline value_type* allocate(std::size_t n) {
//      std::size_t byte_size = n * sizeof(value_type);
//      fst_assert(_index + byte_size <= _Size, "Out of memory.");
//      value_type* ptr = _buffer.data() + _index;
//      _index += byte_size;
//      return ptr;
//    }
//
//    void deallocate(value_type*, std::size_t n) {
//      std::size_t byte_size = n * sizeof(value_type);
//      fst_assert(byte_size <= _index, "Deallocating too much memory.");
//      _index -= byte_size;
//    }
//
//  private:
//    std::array<value_type, _Size> _buffer;
//    std::size_t _index = 0;
//  };
//
//  template <typename T, typename U, std::size_t _Size>
//  inline bool operator==(const small_string_allocator<T, _Size>&, const small_string_allocator<U, _Size>&) {
//    return true;
//  }
//
//  template <typename T, typename U, std::size_t _Size>
//  inline bool operator!=(const small_string_allocator<T, _Size>&, const small_string_allocator<U, _Size>&) {
//    return false;
//  }
//
//  template <std::size_t _Size>
//  using string = std::basic_string<char, std::char_traits<char>, small_string_allocator<char, _Size>>;
//
//  template <std::size_t _Size>
//  using stringstream = std::basic_stringstream<char, std::char_traits<char>, small_string_allocator<char, _Size>>;
//} // namespace small_string_detail.
//
// template <std::size_t _Size = 32>
// class small_string : public small_string_detail::string<_Size> {
// public:
//  using base_type = small_string_detail::string<_Size>;
//  using base_type::base_type;
//  using base_type::operator=;
//  using base_type::size;
//
//  small_string(const small_string_detail::string<_Size>& s)
//      : base_type(s) {}
//
//  inline std::string to_str_string() const { return std::string(base_type::data(), base_type::size()); }
//  inline operator std::string() const { return std::string(base_type::data(), base_type::size()); }
//};
//
// template <std::size_t _Size>
// inline small_string<_Size> operator+(const small_string<_Size>& lhs, const small_string<_Size>& rhs) {
//  return lhs + rhs;
//}
//
// template <std::size_t _Size = 32>
// using small_stringstream = small_string_detail::stringstream<_Size>;
//
// template <typename T, std::size_t _Size>
// inline T from_string(const char* str, std::size_t size) {
//  small_stringstream<_Size> ss;
//  ss << std::string_view(str, size);
//  T result;
//  ss >> result;
//  return result;
//}
//
// template <typename T, std::size_t _Size>
// inline T from_string(const char* str) {
//  small_stringstream<_Size> ss;
//  ss << std::string_view(str);
//  T result;
//  ss >> result;
//  return result;
//}
//
// template <typename T, std::size_t _Size>
// inline T from_string(const std::string_view& str) {
//  small_stringstream<_Size> ss;
//  ss << str;
//  T result;
//  ss >> result;
//  return result;
//}
//
// template <typename T, std::size_t _Size>
// inline std::pair<bool, T> to_number(std::string_view str) {
//  T value;
//
//  small_stringstream<_Size> stream;
//  stream << str;
//  stream >> value;
//  return std::pair<bool, T>{ !stream.fail(), value };
//}
} // namespace fst.
