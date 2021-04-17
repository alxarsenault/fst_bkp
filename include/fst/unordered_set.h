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
#include "fst/span.h"
#include <algorithm>
#include <vector>

namespace fst {
template <typename _T>
class unordered_set {
public:
  using value_type = _T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using const_pointer = const value_type*;
  using const_iterator = const_pointer;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  static_assert(std::is_integral<value_type>::value, "Integral type required.");

  inline size_type size() const { return _content_size; }

  inline size_type maximum_size() const { return _maximum_size; }

  inline bool empty() const noexcept { return _content_size == 0; }

  inline void resize(size_type __size) {
    fst_assert(_maximum_size >= _content_size,
        "unordered_set::resize Can't downsize when content size is greather than size.");

    _content.resize(__size);
    _is_in_content.resize(__size, false);
    _maximum_size = __size;
  }

  inline void insert(value_type value) {
    fst_assert(value < _maximum_size, "unordered_set::insert Out of bound value.");
    if (_is_in_content[value]) {
      return;
    }

    _content[_content_size] = value;
    _content_size++;
    _is_in_content[value] = true;
  }

  void erase(value_type value) {
    fst_assert(value < _maximum_size, "unordered_set::erase Out of bound value.");
    if (!_is_in_content[value]) {
      return;
    }

    _is_in_content[value] = false;

    size_type f_index = std::numeric_limits<size_type>::max();
    for (size_type i = 0; i < _content_size; i++) {
      if (_content[i] == value) {
        f_index = i;
        break;
      }
    }

    fst_assert(f_index != std::numeric_limits<size_type>::max(), "unordered_set::erase Could not find value.");

    if (f_index != _content_size) {
      _content[f_index] = std::move(_content[_content_size - 1]);
    }

    _content_size--;
  }

  inline bool contains(value_type value) const noexcept {
    fst_assert(value < _maximum_size, "unordered_set::contains Out of bound value.");
    return _is_in_content[value];
  }

  void clear() noexcept {
    std::fill(_is_in_content.begin(), _is_in_content.end(), false);
    _content_size = 0;
  }

  inline fst::span<const value_type> content() const {
    return fst::span<const value_type>(&_content[0], _content_size);
  }

  inline const_reference operator[](size_type index) const { return _content[index]; }

  inline const_iterator begin() const noexcept { return _content.begin(); }
  inline const_iterator end() const noexcept { return _content.begin() + _content_size; }

private:
  std::vector<value_type> _content;
  std::vector<bool> _is_in_content;
  size_type _maximum_size = 0;
  size_type _content_size = 0;
};
} // namespace fst.
