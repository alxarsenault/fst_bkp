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

#include "fst/ui/path.h"
#include "fst/ui/image.h"

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>
namespace fst::ui {
using native_context = CGContextRef;
//using const_native_path = CGPathRef;
using native_context_value_type = CGFloat;
} // namespace fst::ui.
#endif // __APPLE__

namespace fst::ui {
class context {
public:
  using value_type = native_context_value_type;
  struct reset_transform_tag{};

  inline context(native_context c);
  inline context(native_context c, reset_transform_tag);

  inline context(native_context c, int top_level_height);

  inline ~context();
  
  inline void save_state();
  inline void restore_state();
  
  inline void translate(const fst::ui::fpoint& pos);
  
  inline void clip_to_rect(const fst::ui::frect& rect);
  inline fst::ui::frect get_clipping_rect();

  inline void set_color(const fst::ui::color& c);
  inline void fill_path(const fst::ui::path& p);
  inline void draw_image(const fst::ui::image& img, const fst::ui::frect& rect);
  inline void banana(const fst::ui::path& p, const fst::ui::fpoint& pos);
  
private:
  native_context _context;
  
  inline void init();
  inline void init(int top_level_height);
  
  
};

#ifdef __APPLE__
inline void flip(native_context c, float flip_height) {
  CGContextConcatCTM(c, CGAffineTransformMake(1.0f, 0.0f, 0.0f, -1.0f, 0.0f, flip_height));
}

context::context(native_context c)
    : _context(c)
  {
    init();
  }
  
  inline context::context(native_context c, reset_transform_tag)
   : _context(c) {
      CGContextSaveGState(_context);
    CGAffineTransform t = CGContextGetCTM(_context);
    t = CGAffineTransformInvert(t);
    CGContextConcatCTM(_context, t);
    CGPoint pp = CGContextConvertPointToUserSpace(_context, CGPoint{0, 0});
    CGContextRestoreGState(_context);
    CGSize ss = CGContextConvertSizeToUserSpace(_context, CGSize{0, pp.y});

    int height = std::abs(ss.height);
    init(height);
  }

  context::context(native_context c, int top_level_height)
    : _context(c)
  {
    init(top_level_height);
  }

  context::~context() {
    CGContextRestoreGState(_context);
  }
  
  inline void context::save_state() {
    CGContextSaveGState(_context);
  }

  inline void context::restore_state() {
    CGContextRestoreGState(_context);
  }
  
  inline void context::translate(const fst::ui::fpoint& pos) {
    CGContextTranslateCTM(_context, pos.x, pos.y);
  }
  
  inline void context::clip_to_rect(const fst::ui::frect& rect) {
    CGContextClipToRect(_context, CGRectMake(rect.x, rect.y, rect.width, rect.height));
  }
  
  inline fst::ui::frect context::get_clipping_rect() {
    CGRect r = CGContextGetClipBoundingBox(_context);
    return fst::ui::frect(r.origin.x, r.origin.y, r.size.width, r.size.width);
  }

  inline void context::set_color(const fst::ui::color& c) {
    CGContextSetRGBFillColor(_context, c.f_red(), c.f_green(), c.f_blue(), c.f_alpha());
    CGContextSetAlpha(_context, 1);
  }
  
  inline void context::fill_path(const fst::ui::path& p) {
    CGContextAddPath(_context, p.get_native_path());
    CGContextFillPath(_context);
  }
  
  inline void context::draw_image(const fst::ui::image& img, const fst::ui::frect& rect) {
    flip(_context, rect.height);
    CGContextDrawImage(_context, CGRectMake(rect.x, rect.y, rect.width, rect.height), img.get_native_image());
    flip(_context, rect.height);
  }
  
  inline void context::banana(const fst::ui::path& p, const fst::ui::fpoint& pos) {
    CGLayerRef layer = CGLayerCreateWithContext(_context, CGSize{100, 100}, nullptr);
    context cc(CGLayerGetContext(layer));
    
    cc.set_color(0x00FF00FF);
    cc.fill_path(p);
    CGContextDrawLayerAtPoint(_context, CGPoint{pos.x, pos.y}, layer);
  }

  inline void context::init() {
    CGContextSaveGState(_context);
  }

  inline void context::init(int top_level_height) {
    CGContextSaveGState(_context);
    CGContextTranslateCTM(_context, 0.0f, top_level_height);
    CGContextScaleCTM(_context, 1.0f, -1.0f);
  }
  #endif // __APPLE__
} // namespace fst::ui.
