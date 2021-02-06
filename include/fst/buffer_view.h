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
#include <cstddef>

namespace fst {
template <typename _Tp>
class buffer_view {
public:
  using value_type = _Tp;
  using pointer = const value_type*;
  using reference = const value_type&;
  using size_type = std::size_t;

  inline buffer_view(pointer data = nullptr, size_type size = 0) noexcept
      : _data(data)
      , _size(size) {}

  buffer_view(const buffer_view&) noexcept = default;
  buffer_view(buffer_view&& bv) noexcept
      : _data(bv._data)
      , _size(bv._size) {
    bv._data = nullptr;
    bv._size = 0;
  }

  buffer_view& operator=(const buffer_view&) noexcept = default;
  buffer_view& operator=(buffer_view&& bv) noexcept {
    _data = bv._data;
    _size = bv._size;
    bv._data = nullptr;
    bv._size = 0;
    return *this;
  }

  inline size_type size() const noexcept { return _size; }
  inline reference operator[](size_type index) const noexcept { return _data[index]; }
  inline pointer data() const noexcept { return _data; }
  inline pointer begin() const noexcept { return _data; }
  inline pointer end() const noexcept { return _data + _size; }

private:
  pointer _data;
  size_type _size;
};
} // namespace fst.
