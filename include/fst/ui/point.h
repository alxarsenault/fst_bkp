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
  class point {
  public:
    using value_type = _Tp;
    using pair_type = std::pair<value_type, value_type>;
    static_assert(std::is_arithmetic<value_type>::value, "value_type is not arithmetic");

    value_type x, y;

    inline constexpr point(value_type X = 0, value_type Y = 0) noexcept {
      x = X;
      y = Y;
    }

    template <typename T>
    inline constexpr point(T X = 0, T Y = 0) noexcept {
      x = (value_type)X;
      y = (value_type)Y;
    }

    template <typename T>
    inline constexpr point(const point<T>& pt) noexcept {
      x = (value_type)pt.x;
      y = (value_type)pt.y;
    }

    inline constexpr point(const pair_type& p) noexcept
        : x(p.first)
        , y(p.second) {}

    point(const std::string& size_str)
        : x(-1)
        , y(-1) {

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

      x = (value_type)std::stod(size_elem[0]);
      y = (value_type)std::stod(size_elem[1]);
    }

    inline constexpr point& operator=(const point& pt) noexcept {
      x = pt.x;
      y = pt.y;
      return *this;
    }

    template <typename T>
    inline constexpr point& operator=(const point<T>& pt) noexcept {
      x = (value_type)pt.x;
      y = (value_type)pt.y;
      return *this;
    }

    inline constexpr pair_type to_pair() const noexcept { return pair_type(x, y); }

    template <typename P>
    inline constexpr point<P> cast() const noexcept {
      return point<P>((P)x, (P)y);
    }

    inline constexpr point operator*(value_type v) const noexcept { return point(x * v, y * v); }
    inline constexpr point& operator*=(value_type v) noexcept {
      x *= v;
      y *= v;
      return *this;
    }

    inline constexpr point operator/(value_type v) const noexcept { return point(x / v, y / v); }
    inline constexpr point& operator/=(value_type v) noexcept {
      x /= v;
      y /= v;
      return *this;
    }

    inline constexpr point operator+(value_type v) const noexcept { return point(x + v, y + v); }
    inline constexpr point& operator+=(value_type v) noexcept {
      x += v;
      y += v;
      return *this;
    }

    inline constexpr point operator-(value_type v) const noexcept { return point(x - v, y - v); }
    inline constexpr point& operator-=(value_type v) noexcept {
      x -= v;
      y -= v;
      return *this;
    }

    inline constexpr point& operator=(value_type v) noexcept {
      x = v;
      y = v;
      return *this;
    }

    inline constexpr point operator*(const point& pt) const noexcept { return point(x * pt.x, y * pt.y); }
    inline constexpr point& operator*=(const point& pt) noexcept {
      x *= pt.x;
      y *= pt.y;
      return *this;
    }

    inline constexpr point operator/(const point& pt) const noexcept { return point(x / pt.x, y / pt.y); }
    inline constexpr point& operator/=(const point& pt) noexcept {
      x /= pt.x;
      y /= pt.y;
      return *this;
    }

    template <typename T>
    inline constexpr point operator+(const point<T>& pt) const noexcept {
      return point(x + (value_type)pt.x, y + (value_type)pt.y);
    }

    inline constexpr point& operator+=(const point& pt) noexcept {
      x += pt.x;
      y += pt.y;
      return *this;
    }

    template <typename T>
    inline constexpr point operator-(const point<T>& pt) const noexcept {
      return point(x - (value_type)pt.x, y - (value_type)pt.y);
    }
    inline constexpr point& operator-=(const point& pt) noexcept {
      x -= pt.x;
      y -= pt.y;
      return *this;
    }

    inline constexpr bool operator==(const point& pt) const noexcept { return (x == pt.x && y == pt.y); }
    inline constexpr bool operator!=(const point& pt) const noexcept { return !(x == pt.x && y == pt.y); }
    inline constexpr bool operator<(const point& pt) const noexcept { return (x < pt.x && y < pt.y); }
    inline constexpr bool operator<=(const point& pt) const noexcept { return (x <= pt.x && y <= pt.y); }
    inline constexpr bool operator>(const point& pt) const noexcept { return (x > pt.x && y > pt.y); }
    inline constexpr bool operator>=(const point& pt) const noexcept { return (x >= pt.x && y >= pt.y); }

    std::string to_string() const { return std::to_string(x) + ", " + std::to_string(y); }

    friend std::ostream& operator<<(std::ostream& stream, const point& point) {
      stream << point.x << ", " << point.y;
      return stream;
    }
  };
} // namespace detail.

using point = detail::point<int>;
using fpoint = detail::point<float>;
} // namespace fst::ui.
