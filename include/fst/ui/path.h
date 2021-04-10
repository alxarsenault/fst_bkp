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
#include "fst/range.h"
#include "fst/math.h"
#include "fst/ui/color.h"
#include "fst/ui/point.h"
#include "fst/ui/size.h"
#include "fst/ui/rect.h"

#include <span>

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>
namespace fst::ui {
using native_path = CGMutablePathRef;
using const_native_path = CGPathRef;
using native_path_value_type = CGFloat;
} // namespace fst::ui.
#endif // __APPLE__

namespace fst::ui {
class path {
public:
  using value_type = native_path_value_type;
  using point = fst::ui::detail::point<value_type>;
  using size = fst::ui::detail::size<value_type>;
  using rect = fst::ui::detail::rect<value_type>;
  using range = fst::range<value_type>;

  inline path();
  
  inline path(const path& p);
  
  inline path(path&& p);
  
  inline ~path();
  
  inline path& operator=(const path& p);
  
  inline path& operator=(path&& p);
  
  inline bool operator==(const path& p) const;

  inline bool operator!=(const path& p) const;

  inline explicit operator bool() const;
  
  inline const_native_path get_native_path() const;

  inline bool is_empty() const;

  inline point get_current_point() const;

  /// The bounding box is the smallest rectangle completely enclosing all points in the path,
  /// including control points for Bézier and quadratic curves.
  inline rect get_bounding_box() const;

  /// The path bounding box is the smallest rectangle completely enclosing all points in the path but
  /// not including control points for Bézier and quadratic curves.
  inline rect get_path_bounding_box() const;
  
  inline bool contains(const point& p) const;

  inline void start_subpath(const point& p);
  inline void close_subpath();
  inline void line_to(const point& p);
  inline void add_line(const point& p0, const point& p1);

  /// This is a convenience function that adds a sequence of connected line segments to a path.
  /// This function is equivalent to calling the 'start_subpath' function to move to the starting
  /// point of the first line segment, then repeatedly calling the 'line_to'
  /// to append the sequence of line segments.
  inline void add_lines(const std::span<const point>& pts);

  /// Appends a cubic Bézier curve from the current point in a path to the specified location using two control
  /// points, after an optional transformation. Before returning, this function updates the current point to the
  /// specified location (x,y).
  inline void add_curve_to_point(const point& ctrl_0, const point& ctrl_1, const point& end_point);
  
  /// Appends a quadratic Bézier curve.
  inline void add_curve_to_point(const point& ctrl, const point& end_point);
  inline void add_rect(const rect& r);
  inline void add_rects(const std::span<const rect>& rects);
  inline void add_rounded_rect(const rect& r, value_type corner_size) ;
  inline void add_rounded_rect(const rect& r, const size& corner_size);

  /// @param angle_range in radians.
  inline void add_arc(const point& center, value_type radius, const range& angle_range, bool clockwise = true);

  inline void add_circle(const point& center, value_type radius);

  inline void add_ellipse(const rect& r);
  
  inline void add_triangle(const point& p0, const point& p1, const point& p2);
  
  inline void add_path(const path& p);

private:
  native_path _path = nullptr;
};

#ifdef __APPLE__
static inline CGPoint to_cg_point(const path::point& p) {
  return *(const CGPoint*)(&p);
}

static inline CGRect to_cg_rect(const path::rect& r) {
  return *(const CGRect*)(&r);
}

static inline path::point to_fst_point(const CGPoint& p) {
  return *(const path::point*)(&p);
}

static inline path::rect to_fst_rect(const CGRect& r) {
  return *(const path::rect*)(&r);
}

inline path::path() {
    _path = CGPathCreateMutable();
  }
  
  inline path::path(const path& p) {
    fst_assert(p._path != nullptr, "Invalid path.");
    _path = CGPathCreateMutableCopy(p._path);
  }
  
  inline path::path(path&& p) {
    _path = p._path;
    p._path = nullptr;
  }
  
  inline path::~path() {
    if(_path) {
      CGPathRelease(_path);
    }
  }
  
  inline path& path::operator=(const path& p) {
    if(_path) {
      CGPathRelease(_path);
      _path = nullptr;
    }

    fst_assert(p._path != nullptr, "Invalid path.");
    _path = CGPathCreateMutableCopy(p._path);
    return *this;
  }
  
  inline path& path::operator=(path&& p) {
    if(_path) {
      CGPathRelease(_path);
      _path = nullptr;
    }

    _path = p._path;
    p._path = nullptr;
    return *this;
  }
  
  inline bool path::operator==(const path& p) const {
    fst_assert(_path != nullptr, "Invalid path.");
    fst_assert(p._path != nullptr, "Invalid path.");
    return CGPathEqualToPath(_path, p._path);
  }

  inline bool path::operator!=(const path& p) const {
    return !operator==(p);
  }

  inline path::operator bool() const {
    return !is_empty();
  }
  
  inline const_native_path path::get_native_path() const {
    return _path;
  }

  //
  //
  //
  inline bool path::is_empty() const {
    return CGPathIsEmpty(_path);
  }

  inline path::point path::get_current_point() const {
    return to_fst_point(CGPathGetCurrentPoint(_path));
  }

  
  inline path::rect path::get_bounding_box() const {
    fst_assert(_path != nullptr, "Invalid path.");
    return to_fst_rect(CGPathGetBoundingBox(_path));
  }

  inline path::rect path::get_path_bounding_box() const {
    fst_assert(_path != nullptr, "Invalid path.");
    return to_fst_rect(CGPathGetPathBoundingBox(_path));
  }
  
  inline bool path::contains(const path::point& p) const {
    fst_assert(_path != nullptr, "Invalid path.");
    return CGPathContainsPoint(_path, nullptr, to_cg_point(p), false);
  }

  inline void path::start_subpath(const path::point& p) {
    fst_assert(_path != nullptr, "Invalid path.");
    CGPathMoveToPoint(_path, nullptr, p.x, p.y);
  }
  
  inline void path::close_subpath() {
    fst_assert(_path != nullptr, "Invalid path.");
    CGPathCloseSubpath(_path);
  }

  inline void path::line_to(const path::point& p) {
    fst_assert(_path != nullptr, "Invalid path.");
    CGPathAddLineToPoint(_path, nullptr, p.x, p.y);
  }

  inline void path::add_line(const path::point& p0, const path::point& p1) {
    fst_assert(_path != nullptr, "Invalid path.");
    CGPathMoveToPoint(_path, nullptr, p0.x, p0.y);
    CGPathAddLineToPoint(_path, nullptr, p1.x, p1.y);
  }

  // This is a convenience function that adds a sequence of connected line segments to a path.
  // This function is equivalent to calling the 'start_subpath' function to move to the starting
  // point of the first line segment, then repeatedly calling the 'line_to'
  // to append the sequence of line segments.
  inline void path::add_lines(const std::span<const path::point>& pts) {
    fst_assert(_path != nullptr, "Invalid path.");
    CGPathAddLines(_path, nullptr, (const CGPoint*)pts.data(), pts.size());
  }

  inline void path::add_curve_to_point(const path::point& ctrl_0, const path::point& ctrl_1, const path::point& end_point) {
    CGPathAddCurveToPoint(_path, nullptr, ctrl_0.x, ctrl_0.y, ctrl_1.x, ctrl_1.y, end_point.x, end_point.y);
  }

  inline void path::add_curve_to_point(const path::point& ctrl, const path::point& end_point) {
    CGPathAddQuadCurveToPoint(_path, nullptr, ctrl.x, ctrl.y, end_point.x, end_point.y);
  }
  
  inline void path::add_rect(const path::rect& r) {
    fst_assert(_path != nullptr, "Invalid path.");
    CGPathAddRect(_path, nullptr, to_cg_rect(r));
  }
  
  inline void path::add_rects(const std::span<const path::rect>& rects) {
    fst_assert(_path != nullptr, "Invalid path.");
    CGPathAddRects(_path, nullptr, (const CGRect*)rects.data(), rects.size());
  }
  
  inline void path::add_rounded_rect(const path::rect& r, value_type corner_size) {
    fst_assert(_path != nullptr, "Invalid path.");
    CGPathAddRoundedRect(_path, nullptr, to_cg_rect(r), corner_size, corner_size);
  }

  inline void path::add_rounded_rect(const path::rect& r, const path::size& corner_size) {
    fst_assert(_path != nullptr, "Invalid path.");
    CGPathAddRoundedRect(_path, nullptr, to_cg_rect(r), corner_size.width, corner_size.height);
  }

  /// @param angle_range in radians.
  inline void path::add_arc(const path::point& center, value_type radius, const path::range& angle_range, bool clockwise) {
    CGPathAddArc(_path, nullptr, center.x, center.y, radius, angle_range.min, angle_range.max, clockwise);
  }

  inline void path::add_circle(const path::point& center, value_type radius) {
    CGPathAddArc(_path, nullptr, center.x, center.y, radius, 0, fst::math::two_pi<value_type>, true);
  }

  inline void path::add_ellipse(const path::rect& r) {
    CGPathAddEllipseInRect(_path, nullptr, to_cg_rect(r));
  }
  
  inline void path::add_triangle(const path::point& p0, const path::point& p1, const path::point& p2) {
    start_subpath(p0);
    line_to(p1);
    line_to(p2);
    close_subpath();
  }
  
  inline void path::add_path(const path& p) {
    fst_assert(_path != nullptr, "Invalid path.");
    fst_assert(p._path != nullptr, "Invalid path.");
    CGPathAddPath(_path, nullptr, p._path);
  }
#endif // __APPLE__.
} // namespace fst::ui.
