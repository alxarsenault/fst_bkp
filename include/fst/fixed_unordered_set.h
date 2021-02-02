#pragma once

#include "fst/assert.h"
#include "fst/spin_lock.h"
#include "fst/unordered_array.h"
#include <bitset>
#include <atomic>

namespace fst {
template <typename _T, std::size_t _Size>
class fixed_unordered_set {
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

  void insert(value_type value) {
    fst_assert(value < maximum_size, "fixed_unordered_set::insert Out of bound value.");
    if (_is_in_array[value]) {
      return;
    }

    _array.push_back(value);
    _is_in_array.set(value);
  }

  void erase(value_type value) {
    fst_assert(value < maximum_size, "fixed_unordered_set::erase Out of bound value.");
    if (!_is_in_array[value]) {
      return;
    }

    _is_in_array.reset(value);
    _array.erase_first_if([value](value_type v) { return v == value; });
  }

  bool contains(value_type value) const noexcept {
    fst_assert(value < maximum_size, "fixed_unordered_set::contains Out of bound value.");
    return _is_in_array[value];
  }

  void clear() noexcept {
    _is_in_array.reset();
    _array.clear();
  }

  inline array_type get_and_clear() {
    array_type content = _array;
    clear();
    return content;
  }

  inline const_iterator begin() const noexcept { return _array.begin(); }

  inline const_iterator end() const noexcept { return _array.end(); }

  inline size_type size() const noexcept { return _array.size(); }

  inline bool empty() const noexcept { return _array.empty(); }

private:
  array_type _array;
  std::bitset<maximum_size> _is_in_array;
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

  void insert(value_type value) {
    fst_assert(value < maximum_size, "lock_free_fixed_unordered_set::insert Out of bound value.");

    scoped_spin_lock lock(_mutex);
    if (_is_in_array[value]) {
      return;
    }

    _array.push_back(value);
    _is_in_array.set(value);
  }

  void erase(value_type value) {
    fst_assert(value < maximum_size, "lock_free_fixed_unordered_set::erase Out of bound value.");

    scoped_spin_lock lock(_mutex);
    if (!_is_in_array[value]) {
      return;
    }

    _is_in_array.reset(value);
    _array.erase_first_if([value](value_type v) { return v == value; });
  }

  bool contains(value_type value) const {
    fst_assert(value < maximum_size, "lock_free_fixed_unordered_set::contains Out of bound value.");

    scoped_spin_lock lock(_mutex);
    return _is_in_array[value];
  }

  void clear() noexcept {
    scoped_spin_lock lock(_mutex);
    _is_in_array.reset();
    _array.clear();
  }

  inline array_type get_content_and_clear() noexcept {
    scoped_spin_lock lock(_mutex);
    array_type content = _array;
    _is_in_array.reset();
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
  std::bitset<maximum_size> _is_in_array;
  mutable spin_lock_mutex _mutex;
};
} // namespace fst.
