#pragma once

#include <iostream>
#include <type_traits>
#include <vector>
#include <string>

namespace fst::ui {
namespace detail {
  template <typename _Tp>
  class padding {
  public:
    using value_type = _Tp;
    static_assert(std::is_arithmetic<value_type>::value, "value_type is not arithmetic");

    value_type left, right, top, bottom;

    inline constexpr padding(value_type Left = 0, value_type Right = 0, value_type Top = 0, value_type Bottom = 0)
        : left(Left)
        , right(Right)
        , top(Top)
        , bottom(Bottom) {}

    template <typename T>
    inline constexpr padding(T Left = 0, T Right = 0, T Top = 0, T Bottom = 0)
        : left(Left)
        , right(Right)
        , top(Top)
        , bottom(Bottom) {}

    padding(const std::string& padding_str)
        : padding() {
      // Split string.
      std::vector<std::string> elem;
      std::string r = padding_str;
      size_t pos = 0;
      std::string token;
      const std::string delimiter(",");

      while ((pos = r.find(delimiter)) != std::string::npos) {
        token = r.substr(0, pos);
        elem.push_back(token);
        r.erase(0, pos + delimiter.length());
      }

      elem.push_back(r);

      if (elem.size() != 4) {
        return;
      }

      left = (value_type)std::stod(elem[0]);
      right = (value_type)std::stod(elem[1]);
      top = (value_type)std::stod(elem[2]);
      bottom = (value_type)std::stod(elem[3]);
    }

    inline constexpr padding& operator=(const padding& p) {
      left = p.left;
      right = p.right;
      top = p.top;
      bottom = p.bottom;
      return *this;
    }

    template <typename T>
    inline constexpr padding& operator=(const padding<T>& p) {
      left = p.left;
      right = p.right;
      top = p.top;
      bottom = p.bottom;
      return *this;
    }

    template <typename P>
    inline constexpr padding<P> cast() const {
      return padding<P>((P)left, (P)right, (P)top, (P)bottom);
    }

    inline constexpr bool operator==(const padding& p) {
      return (left == p.left && right == p.right && top == p.top && bottom == p.bottom);
    }
    inline constexpr bool operator!=(const padding& p) { return !this->operator==(p); }

    std::string to_string() const {
      return std::to_string(left) + ", " + std::to_string(right) + ", " + std::to_string(top) + ", "
          + std::to_string(bottom);
    }

    friend std::ostream& operator<<(std::ostream& stream, const padding& p) {
      stream << p.left << ", " << p.right << ", " << p.top << ", " << p.bottom;
      return stream;
    }
  };
} // namespace detail.

using padding = detail::padding<int>;
using fpadding = detail::padding<float>;
} // namespace fst::ui.
