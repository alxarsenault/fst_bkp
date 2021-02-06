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
#include <array>
#include <exception>
#include <initializer_list>
#include <type_traits>

namespace fst {
template <typename _Tp, std::size_t _Size>
class unordered_array {
public:
  using value_type = _Tp;

private:
  static constexpr bool has_const_reference = std::is_fundamental<value_type>::value;

  template <bool _Dummy, class _D = dependent_type_condition<_Dummy, std::bool_constant<has_const_reference>>>
  using enable_if_has_const_reference = enable_if_same<_Dummy, _D>;

public:
  using array_type = std::array<value_type, _Size>;
  using reference = typename array_type::reference;
  using const_reference = std::conditional_t<has_const_reference, typename array_type::const_reference, value_type>;
  using pointer = typename array_type::pointer;
  using const_pointer = typename array_type::const_pointer;
  using iterator = typename array_type::iterator;
  using const_iterator = typename array_type::const_iterator;
  using reverse_iterator = typename array_type::reverse_iterator;
  using const_reverse_iterator = typename array_type::const_reverse_iterator;

  using size_type = typename array_type::size_type;
  using difference_type = typename array_type::difference_type;

  static_assert(_Size != 0, "unordered_array size must not be zero.");

  inline unordered_array() noexcept = default;
  inline unordered_array(const unordered_array&) noexcept = default;
  inline unordered_array(unordered_array&& ua) noexcept
      : _data(std::move(ua._data))
      , _size(ua._size) {
    ua._size = 0;
  }

  inline unordered_array(std::initializer_list<value_type> list) {
    for (auto n : list) {
      push_back(maybe_move(n));
    }
  }

  inline unordered_array& operator=(const unordered_array&) noexcept = default;
  inline unordered_array& operator=(unordered_array&& ua) noexcept {
    _data = std::move(ua._data);
    _size = ua._size;
    ua._size = 0;
    return *this;
  }

  inline size_type size() const noexcept { return _size; }
  inline constexpr size_type max_size() const noexcept { return _Size; }
  inline bool empty() const noexcept { return _size == 0; }
  inline pointer data() noexcept { return _data.data(); }
  inline const_pointer data() const noexcept { return _data.data(); }

  inline reference operator[](size_type index) noexcept {
    fst_assert(index < _size, "Try to access out of bounds index.");
    return _data[index];
  }

  inline const_reference operator[](size_type index) const noexcept {
    fst_assert(index < _size, "Try to access out of bounds index.");
    return _data[index];
  }

  inline reference at(size_type index) {
    if (index >= max_size()) {
      throw std::out_of_range("unordered_array::at");
    }

    return _data[index];
  }

  inline const_reference at(size_type index) const {
    if (index >= max_size()) {
      throw std::out_of_range("unordered_array::at");
    }

    return _data[index];
  }

  inline iterator begin() { return _data.begin(); }
  inline const_iterator begin() const { return _data.begin(); }
  inline iterator end() { return _data.begin() + _size; }
  inline const_iterator end() const { return _data.begin() + _size; }

  inline reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  inline const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
  inline reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  inline const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

  inline const_iterator cbegin() const noexcept { return begin(); }
  inline const_iterator cend() const noexcept { return end(); }
  inline const_reverse_iterator crbegin() const noexcept { return rbegin(); }
  inline const_reverse_iterator crend() const noexcept { return rend(); }

  inline reference front() noexcept { return _data[0]; }
  inline const_reference front() const noexcept { return _data[0]; }
  inline reference back() noexcept { return _data[_size - 1]; }
  inline const_reference back() const noexcept { return _data[_size - 1]; }

  inline void swap(unordered_array& ua) noexcept {
    _data.swap(ua._data);
    size_type temp_size = _size;
    _size = ua._size;
    ua._size = temp_size;
  }

  inline void clear() {
    if constexpr (!std::is_trivial<value_type>::value) {
      for (size_type i = 0; i < _size; i++) {
        _data[i] = value_type();
      }
    }

    _size = 0;
  }

  inline bool is_full() const { return _size == max_size(); }

  inline void push_back(const_reference value) {
    fst_assert(_size != max_size(), "Can't push_back when array is full.");

    if (_size == max_size()) {
      return;
    }

    _data[_size++] = value;
  }

  // Doesn't apply for fundamental types.
  template <bool _Dummy = true, class = enable_if_has_const_reference<_Dummy>>
  inline void push_back(value_type&& value) {
    fst_assert(_size != max_size(), "Can't push_back when array is full.");

    if (_size == max_size()) {
      return;
    }

    _data[_size++] = std::move(value);
  }

  template <typename... Args>
  inline void emplace_back(Args&&... args) {
    fst_assert(_size != max_size(), "Can't emplace_back when array is full.");

    if (_size == max_size()) {
      return;
    }

    _data[_size++] = value_type(std::forward<Args>(args)...);
  }

  inline void pop_back() {
    fst_assert(_size > 0, "Can't pop_back an empty array.");

    if (_size == 0) {
      return;
    }

    if (_size) {
      --_size;

      if constexpr (!std::is_trivial<value_type>::value) {
        _data[_size] = value_type();
      }
    }
  }

  inline void erase(size_type index) {
    fst_assert(index < _size, "Try to erase out of bounds index.");

    if (_size == 0) {
      return;
    }

    if (index == _size - 1) {
      _size--;
      return;
    }

    _data[index] = std::move(_data[_size-- - 1]);
  }

  template <class Predicate>
  inline void erase_if(Predicate predicate) {
    for (size_type i = 0; i < _size;) {
      if (predicate(_data[i])) {
        erase(i);
        continue;
      }

      i++;
    }
  }

  template <class Predicate>
  inline void erase_first_if(Predicate predicate) {
    for (size_type i = 0; i < _size; i++) {
      if (predicate(_data[i])) {
        erase(i);
        return;
      }
    }
  }

  template <class Operator>
  inline void operation(Operator oper) {
    for (size_type i = 0; i < _size; i++) {
      oper(_data[i]);
    }
  }

  template <class Predicate, class Operator>
  inline void operation_if(Predicate predicate, Operator oper) {
    for (size_type i = 0; i < _size; i++) {
      if (predicate(_data[i])) {
        oper(_data[i]);
      }
    }
  }

  template <class Predicate, class Operator>
  inline void operation_first_if(Predicate predicate, Operator oper) {
    for (size_type i = 0; i < _size; i++) {
      if (predicate(_data[i])) {
        oper(_data[i]);
        return;
      }
    }
  }

private:
  array_type _data;
  size_type _size = 0;
};
} // namespace fst.
