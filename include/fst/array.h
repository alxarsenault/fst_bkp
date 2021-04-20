///
/// BSD 3-Clause License
///
/// Copyright (c) 2021, Alexandre Arsenault
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
#include <cstddef>

namespace fst {
template <class _Tp, std::size_t _Size>
class array {
public:
  static_assert(_Size > 0, "fst::array size can't be zero");
  using value_type = _Tp;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  value_type __elems_[_Size];

  // Iterators.
  inline constexpr iterator begin() noexcept { return iterator(data()); }
  inline constexpr const_iterator begin() const noexcept { return const_iterator(data()); }
  inline constexpr iterator end() noexcept { return iterator(data() + _Size); }
  inline constexpr const_iterator end() const noexcept { return const_iterator(data() + _Size); }

  inline constexpr const_iterator cbegin() const noexcept { return begin(); }
  inline constexpr const_iterator cend() const noexcept { return end(); }

  // Capacity.
  inline constexpr size_type size() const noexcept { return _Size; }
  inline constexpr size_type max_size() const noexcept { return _Size; }
  [[nodiscard]] inline constexpr bool empty() const noexcept { return false; }

  // Element access.
  inline constexpr reference operator[](size_type __n) noexcept { return __elems_[__n]; }
  inline constexpr const_reference operator[](size_type __n) const noexcept { return __elems_[__n]; }

  inline constexpr reference front() noexcept { return __elems_[0]; }
  inline constexpr const_reference front() const noexcept { return __elems_[0]; }
  inline constexpr reference back() noexcept { return __elems_[_Size - 1]; }
  inline constexpr const_reference back() const noexcept { return __elems_[_Size - 1]; }

  inline constexpr value_type* data() noexcept { return __elems_; }
  inline constexpr const value_type* data() const noexcept { return __elems_; }
};
} // namespace fst.
