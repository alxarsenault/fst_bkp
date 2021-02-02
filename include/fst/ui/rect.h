#pragma once

#include "fst/ui/point.h"
#include "fst/ui/size.h"

namespace fst::ui {
namespace detail {
  template <typename _Tp>
  class rect {
  public:
    using value_type = _Tp;
    static_assert(std::is_arithmetic<value_type>::value, "value_type is not arithmetic");
    using size_type = detail::size<value_type>;
    using point_type = detail::point<value_type>;

    union {
      point_type position;
      struct {
        value_type x, y;
      };
    };

    union {
      size_type size;
      struct {
        value_type width, height;
      };
    };

    inline constexpr rect(const point_type& pt_ = point_type(0, 0), const size_type& size_ = size_type(0, 0))
        : position(pt_)
        , size(size_) {}

    inline constexpr rect(value_type x, value_type y, value_type w, value_type h)
        : position(x, y)
        , size(w, h) {}

    inline constexpr rect(value_type x, value_type y, const size_type& size_)
        : position(x, y)
        , size(size_) {}

    inline constexpr rect(const rect& r)
        : position(r.position)
        , size(r.size) {}

    template <typename T>
    inline constexpr rect(const rect<T>& r)
        : position(r.position.template cast<value_type>())
        , size(r.size.template cast<value_type>()) {}

    inline constexpr rect& operator=(const rect& r) {
      position = r.position;
      size = r.size;
      return *this;
    }

    inline constexpr static rect from_bottom_left(value_type x, value_type y, value_type w, value_type h) {
      return rect(x, y - h, w, h);
    }

    template <typename P>
    inline constexpr rect<P> cast() const {
      return rect<P>(position.template cast<P>(), size.template cast<P>());
    }

    inline constexpr rect operator+(const rect& rect) const {
      return rect(position.x + rect.position.x, position.y + rect.position.y, size.width + rect.size.width,
          size.height + rect.size.height);
    }

    inline constexpr rect operator-(const rect& rect) const {
      return rect(position.x - rect.position.x, position.y - rect.position.y, size.width - rect.size.width,
          size.height - rect.size.height);
    }

    inline constexpr bool operator==(const rect& rect) const { return position == rect.position && size == rect.size; }
    inline constexpr bool operator!=(const rect& rect) const { return !operator==(rect); }

    inline constexpr value_type left() const { return position.x; }
    inline constexpr value_type right() const { return position.x + size.width; }
    inline constexpr value_type top() const { return position.y; }

    inline constexpr value_type bottom() const { return position.y + size.height; }

    inline constexpr bool intersect(const rect& r) const {
      return left() > r.right() || r.left() > right() || top() < r.bottom() || r.top() < bottom();
    }

    template <typename _PointValueType>
    inline constexpr bool is_inside(const detail::point<_PointValueType>& pos) const {
      return pos.x >= position.x && pos.x <= position.x + size.width && pos.y >= position.y
          && pos.y <= position.y + size.height;
    }

    inline constexpr rect get_interior_rect(const point_type& point) const {
      return rect(position.x + point.x, position.y + point.y, size.width - (value_type)(2.0 * point.x),
          size.height - (value_type)(2.0 * point.y));
    }

    inline constexpr void reduce(const point_type& point) {
      position.x += point.x;
      position.y += point.y;
      size.width -= (value_type)(2.0 * point.x);
      size.height -= (value_type)(2.0 * point.y);
    }

    inline constexpr rect reduced(const point_type& point) const {
      return rect(x + point.x, y + point.y, width - (value_type)(2.0 * point.x), height - (value_type)(2.0 * point.y));
    }

    inline constexpr rect reduced_from_left_percent(float v) const { return reduced_from_left(width * (float)v); }
    inline constexpr rect reduced_from_right_percent(float v) const { return reduced_from_right(width * (float)v); }

    inline constexpr rect reduced_from_left(value_type x) const {
      return rect(position.x + x, position.y, size.width - x, size.height);
    }
    inline constexpr rect reduced_from_right(value_type x) const {
      return rect(position.x, position.y, size.width - x, size.height);
    }
    inline constexpr rect reduced_from_top(value_type y) const {
      return rect(position.x, position.y + y, size.width, size.height - y);
    }
    inline constexpr rect reduced_from_bottom(value_type y) const {
      return rect(position.x, position.y, size.width, size.height - y);
    }

    inline constexpr point_type in_middle(const size_type& s) const {
      return point_type(x + (width - s.width) * 0.5, y + (height - s.height) * 0.5);
    }

    inline constexpr point_type top_left() const { return position; }
    //  point_type top_right() const { return point_type(position.x + size.width, position.y); }
    inline constexpr point_type top_right(value_type dx = 0, value_type dy = 0) const {
      return point_type(position.x + size.width + dx, position.y + dy);
    }
    inline constexpr point_type bottom_left() const { return point_type(position.x, position.y + size.height); }
    inline constexpr point_type bottom_right() const {
      return point_type(position.x + size.width, position.y + size.height);
    }
    inline constexpr point_type middle() const {
      return point_type(position.x + size.width * 0.5, position.y + size.height * 0.5);
    }
    inline constexpr point_type next_left(value_type delta) const { return point_type(position.x, position.y - delta); }
    inline constexpr point_type next_right(value_type delta) const {
      return point_type(position.x + size.width + delta, position.y);
    }
    inline constexpr point_type next_down(value_type delta) const {
      return point_type(position.x, position.y + size.height + delta);
    }
    inline constexpr point_type next_up(value_type delta) const { return point_type(position.x, position.y - delta); }

    friend std::ostream& operator<<(std::ostream& stream, const rect& rect) {
      stream << rect.position.x << ", " << rect.position.y << ", " << rect.size.width << ", " << rect.size.height;
      return stream;
    }
  };
} // namespace detail.

using rect = detail::rect<int>;
using frect = detail::rect<float>;
} // namespace fst::ui.
