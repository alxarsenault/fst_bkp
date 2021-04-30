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
#include "fst/spin_lock.h"
#include "fst/unordered_array.h"
#include "fst/enum_array.h"
#include <array>
#include <atomic>
#include <initializer_list>

namespace fst {
template <typename _T, std::size_t _Size>
class fixed_unordered_set {
public:
  using value_type = _T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using const_pointer = const value_type*;
  using const_iterator = const_pointer;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  static constexpr size_type maximum_size = _Size;

  using array_type = unordered_array<value_type, maximum_size>;

  static_assert(std::is_integral<value_type>::value, "Integral type required.");

  fixed_unordered_set() { _is_in_array.fill(false); }

  inline void insert(value_type value) {
    fst_assert(value < maximum_size, "fixed_unordered_set::insert Out of bound value.");
    if (_is_in_array[value]) {
      return;
    }

    _array.push_back(value);
    _is_in_array[value] = true;
  }

  inline void erase(value_type value) {
    fst_assert(value < maximum_size, "fixed_unordered_set::erase Out of bound value.");
    if (!_is_in_array[value]) {
      return;
    }

    _is_in_array[value] = false;
    _array.erase_first_if([value](value_type v) { return v == value; });
  }

  inline bool contains(value_type value) const noexcept {
    fst_assert(value < maximum_size, "fixed_unordered_set::contains Out of bound value.");
    return _is_in_array[value];
  }

  inline void clear() noexcept {
    _is_in_array.fill(false);
    _array.clear();
  }

  inline array_type get_and_clear() {
    array_type content = _array;
    clear();
    return content;
  }

  inline const array_type& content() const {
    return _array;
  }

  inline const_reference operator[](size_type index) const { return _array[index]; }

  inline const_iterator begin() const noexcept { return _array.begin(); }
  inline const_iterator end() const noexcept { return _array.end(); }
  inline size_type size() const noexcept { return _array.size(); }
  inline bool empty() const noexcept { return _array.empty(); }

private:
  array_type _array;
  std::array<bool, maximum_size> _is_in_array;
};

template <class _T, class Enum, std::size_t _Size = std::size_t(Enum::count)>
class enum_unordered_set {
public:
  using value_type = _T;
  using enum_type = Enum;
  using reference = value_type&;
  using const_reference = const value_type&;
  using const_pointer = const value_type*;
  using const_iterator = const_pointer;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  static constexpr size_type maximum_size = _Size;

  using array_type = unordered_array<value_type, maximum_size>;

  static_assert(std::is_integral<value_type>::value, "Integral type required.");

  enum_unordered_set() { _is_in_array.fill(false); }

  inline void insert(value_type value) {
    fst_assert(value < maximum_size, "fixed_unordered_set::insert Out of bound value.");
    if (_is_in_array[value]) {
      return;
    }

    _array.push_back(value);
    _is_in_array[value] = true;
  }

  inline void insert(enum_type e) { insert((std::size_t)e); }

  inline void erase(value_type value) {
    fst_assert(value < maximum_size, "fixed_unordered_set::erase Out of bound value.");
    if (!_is_in_array[value]) {
      return;
    }

    _is_in_array[value] = false;
    _array.erase_first_if([value](value_type v) { return v == value; });
  }

  inline void erase(enum_type e) { erase((std::size_t)e); }

  inline bool contains(value_type value) const noexcept {
    fst_assert(value < maximum_size, "fixed_unordered_set::contains Out of bound value.");
    return _is_in_array[value];
  }

  inline bool contains(enum_type e) const noexcept { return contains((std::size_t)e); }

  inline void clear() noexcept {
    _is_in_array.fill(false);
    _array.clear();
  }

  inline array_type get_and_clear() {
    array_type content = _array;
    clear();
    return content;
  }

  inline const array_type& content() const { return _array; }

  inline const_reference operator[](enum_type e) noexcept { return _array[(std::size_t)e]; }
  inline const_reference operator[](size_type index) const { return _array[index]; }

  inline const_iterator begin() const noexcept { return _array.begin(); }
  inline const_iterator end() const noexcept { return _array.end(); }
  inline size_type size() const noexcept { return _array.size(); }
  inline bool empty() const noexcept { return _array.empty(); }

private:
  array_type _array;
  std::array<bool, maximum_size> _is_in_array;
};

template <class Enum, std::size_t _Size = std::size_t(Enum::count)>
class enum_set {
public:
  using value_type = Enum;
  using reference = value_type&;
  using const_reference = const value_type&;
  using const_pointer = const value_type*;
  using const_iterator = const_pointer;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  static constexpr size_type maximum_size = _Size;

  using array_type = unordered_array<value_type, maximum_size>;

  constexpr enum_set() noexcept { _is_in_array.fill(false); }

  inline constexpr void insert(value_type value) noexcept {
    //    fst::print("enum_set:insert(value_type)", std::size_t(value), _is_in_array[value]);
    fst_assert(std::size_t(value) < maximum_size, "fixed_unordered_set::insert Out of bound value.");
    if (_is_in_array[value]) {
      return;
    }

    _array.push_back(value);
    _is_in_array[value] = true;
  }

  inline constexpr void insert(size_type __index) noexcept {
    //    fst::print("enum_set:insert", __index);
    insert((value_type)__index);
  }

  inline constexpr void erase(value_type value) noexcept {
    fst_assert(std::size_t(value) < maximum_size, "fixed_unordered_set::erase Out of bound value.");
    if (!_is_in_array[value]) {
      return;
    }

    _is_in_array[value] = false;
    _array.erase_first_if([value](value_type v) { return v == value; });
  }

  inline constexpr void erase(size_type __index) noexcept { erase((value_type)__index); }

  inline constexpr bool contains(value_type value) const noexcept {
    fst_assert(std::size_t(value) < maximum_size, "fixed_unordered_set::contains Out of bound value.");
    //    fst::print("enum_set:contains(value_type)", std::size_t(value));
    return _is_in_array[value];
  }

  inline constexpr bool contains(size_type __index) const noexcept {
    //    fst::print("enum_set:contains", __index);
    return contains((value_type)__index);
  }

  inline constexpr void clear() noexcept {
    _is_in_array.fill(false);
    _array.clear();
  }

  inline constexpr array_type get_and_clear() noexcept {
    array_type content = _array;
    clear();
    return content;
  }

  inline constexpr const array_type& content() const noexcept { return _array; }

  inline constexpr const_reference operator[](value_type e) noexcept { return _array[(std::size_t)e]; }
  inline constexpr const_reference operator[](size_type __index) const noexcept { return _array[__index]; }

  inline constexpr const_iterator begin() const noexcept { return _array.begin(); }
  inline constexpr const_iterator end() const noexcept { return _array.end(); }
  inline constexpr size_type size() const noexcept { return _array.size(); }
  inline constexpr bool empty() const noexcept { return _array.empty(); }

private:
  array_type _array;
  fst::enum_array<bool, value_type> _is_in_array;
};

template <typename _T, std::size_t _Size>
class lock_free_fixed_unordered_set {
public:
  using value_type = _T;
  using const_reference = const value_type&;
  using const_pointer = const value_type*;
  using const_iterator = const_pointer;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  static constexpr size_type maximum_size = _Size;

  using array_type = unordered_array<value_type, maximum_size>;

  static_assert(std::is_integral<value_type>::value, "Integral type required.");

  inline lock_free_fixed_unordered_set() { _is_in_array.fill(false); }

  inline void insert(value_type value) {
    fst_assert(value < maximum_size, "lock_free_fixed_unordered_set::insert Out of bound value.");

    scoped_spin_lock lock(_mutex);
    if (_is_in_array[value]) {
      return;
    }

    _array.push_back(value);
    _is_in_array[value] = true;
  }

  inline void erase(value_type value) {
    fst_assert(value < maximum_size, "lock_free_fixed_unordered_set::erase Out of bound value.");

    scoped_spin_lock lock(_mutex);
    if (!_is_in_array[value]) {
      return;
    }

    _is_in_array[value] = false;
    _array.erase_first_if([value](value_type v) { return v == value; });
  }

  inline bool contains(value_type value) const {
    fst_assert(value < maximum_size, "lock_free_fixed_unordered_set::contains Out of bound value.");

    scoped_spin_lock lock(_mutex);
    return _is_in_array[value];
  }

  inline void clear() noexcept {
    scoped_spin_lock lock(_mutex);
    _is_in_array.fill(false);
    _array.clear();
  }

  inline array_type get_content_and_clear() noexcept {
    scoped_spin_lock lock(_mutex);
    array_type content = _array;

    // Can't call clear() here to prevent dead lock.
    _is_in_array.fill(false);
    _array.clear();
    return content;
  }

  inline array_type get_content() const noexcept {
    scoped_spin_lock lock(_mutex);
    return _array;
  }

  inline size_type size() const noexcept {
    scoped_spin_lock lock(_mutex);
    return _array.size();
  }

  inline bool empty() const noexcept {
    scoped_spin_lock lock(_mutex);
    return _array.empty();
  }

private:
  array_type _array;
  std::array<bool, maximum_size> _is_in_array;
  mutable spin_lock_mutex _mutex;
};
} // namespace fst.
