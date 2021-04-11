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
#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

namespace fst::ui {
namespace detail {
  template <typename _Tp>
  class size {
  public:
    using value_type = _Tp;
    using pair_type = std::pair<value_type, value_type>;
    static_assert(std::is_arithmetic<value_type>::value, "value_type is not arithmetic");

    value_type width, height;

    inline constexpr size(value_type W = 0, value_type H = 0) noexcept {
      width = W;
      height = H;
    }

    template <typename T>
    inline constexpr size(const size<T>& s) noexcept {
      width = (value_type)s.width;
      height = (value_type)s.height;
    }

    inline constexpr size(const pair_type& p) noexcept
        : width(p.first)
        , height(p.second) {}

    size(const std::string& size_str)
        : width(-1)
        , height(-1) {
      // Split string.
      std::vector<std::string> size_elem;
      std::string r = size_str;
      size_t pos = 0;
      std::string token;
      const std::string delimiter(",");

      while ((pos = r.find(delimiter)) != std::string::npos) {
        token = r.substr(0, pos);
        size_elem.push_back(token);
        r.erase(0, pos + delimiter.length());
      }

      size_elem.push_back(r);

      if (size_elem.size() != 2) {
        return;
      }

      width = (value_type)std::stod(size_elem[0]);
      height = (value_type)std::stod(size_elem[1]);
    }

    inline constexpr size& operator=(const size& size) noexcept {
      width = size.width;
      height = size.height;
      return *this;
    }

    template <typename T>
    inline constexpr size& operator=(const size<T>& size) noexcept {
      width = (value_type)size.width;
      height = (value_type)size.height;
      return *this;
    }

    inline constexpr pair_type to_pair() const noexcept { return pair_type(width, height); }

    template <typename P>
    inline constexpr size<P> cast() const noexcept {
      return size<P>((P)width, (P)height);
    }

    inline constexpr size operator*(const size& size) const noexcept {
      return size(width * size.width, height * size.height);
    }

    inline constexpr size operator*(value_type v) const noexcept { return size(width * v, height * v); }

    inline constexpr size& operator*=(const size& size) noexcept {
      width *= size.width;
      height *= size.height;
      return *this;
    }

    inline constexpr size& operator*=(value_type v) noexcept {
      width *= v;
      height *= v;
      return *this;
    }

    inline constexpr size operator+(const size& size) const noexcept {
      return size(width + size.width, height + size.height);
    }
    inline constexpr size operator-(const size& size) const noexcept {
      return size(width - size.width, height - size.height);
    }

    inline constexpr size& operator+=(const size& size) noexcept {
      width += size.width;
      height += size.height;
      return *this;
    }

    inline constexpr size& operator-=(const size& size) noexcept {
      width -= size.width;
      height -= size.height;
      return *this;
    }

    inline constexpr size& operator-=(value_type v) noexcept {
      width -= v;
      height -= v;
      return *this;
    }

    inline constexpr size& operator=(value_type size) noexcept {
      width = size;
      height = size;
      return *this;
    }

    inline constexpr bool operator==(const size& size) const noexcept {
      return (width == size.width && height == size.height);
    }

    inline constexpr bool operator!=(const size& size) const noexcept {
      return !(width == size.width && height == size.height);
    }
    inline constexpr bool operator<(const size& size) const noexcept { return (width < size.width && height < size.height); }
    inline constexpr bool operator<=(const size& size) const noexcept {
      return (width <= size.width && height <= size.height);
    }
    inline constexpr bool operator>(const size& size) const noexcept  { return (width > size.width && height > size.height); }
    inline constexpr bool operator>=(const size& size) const noexcept {
      return (width >= size.width && height >= size.height);
    }

    std::string to_string() const noexcept { return std::to_string(width) + ", " + std::to_string(height); }

    friend std::ostream& operator<<(std::ostream& stream, const size& size) {
      stream << size.width << ", " << size.height;
      return stream;
    }
  };
} // namespace detail.

using size = detail::size<int>;
using fsize = detail::size<float>;
} // namespace fst::ui.
