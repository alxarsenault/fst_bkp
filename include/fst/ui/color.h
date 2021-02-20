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
#include <cstdint>
#include "fst/util.h"
#include "fst/math.h"

namespace fst::ui {
class color {
public:
  struct float_rgba {
    float r, g, b, a;
  };

  inline static constexpr color black() { return color(0x000000FF); }
  inline static constexpr color white() { return color(0xFFFFFFFF); }

  inline constexpr color() = default;

  inline constexpr color(std::uint32_t rgba)
      : _rgba(rgba) {}

  inline constexpr color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) {
    using u32 = std::uint32_t;
    _rgba = (u32(a) << shift_alpha) | (u32(b) << shift_blue) | (u32(g) << shift_green) | (u32(r) << shift_red);
  }

  inline constexpr color(const float_rgba& rgba) {
    std::uint32_t ur = (std::uint32_t)math::floor(rgba.r * 255);
    std::uint32_t ug = (std::uint32_t)math::floor(rgba.g * 255);
    std::uint32_t ub = (std::uint32_t)math::floor(rgba.b * 255);
    std::uint32_t ua = (std::uint32_t)math::floor(rgba.a * 255);
    _rgba = (ua << shift_alpha) | (ub << shift_blue) | (ug << shift_green) | (ur << shift_red);
  }

  inline constexpr std::uint32_t value() const { return _rgba; }

  inline constexpr std::uint32_t rgba() const { return _rgba; }

#define set_color(color_name, v) _rgba = (_rgba & ~bits_##color_name) | (std::uint32_t(v) << shift_##color_name)
  inline constexpr void set_red(std::uint8_t r) { set_color(red, r); }
  inline constexpr void set_green(std::uint8_t g) { set_color(green, g); }
  inline constexpr void set_blue(std::uint8_t b) { set_color(blue, b); }
  inline constexpr void set_alpha(std::uint8_t a) { set_color(alpha, a); }
#undef set_color

  inline constexpr std::uint8_t red() const { return (_rgba & bits_red) >> shift_red; }
  inline constexpr std::uint8_t green() const { return (_rgba & bits_green) >> shift_green; }
  inline constexpr std::uint8_t blue() const { return (_rgba & bits_blue) >> shift_blue; }
  inline constexpr std::uint8_t alpha() const { return (_rgba & bits_alpha) >> shift_alpha; }

  inline constexpr color darker(float amount) const {
    amount = 1.0f - std::clamp<float>(amount, 0.0f, 1.0f);
    return color(std::uint8_t(red() * amount), std::uint8_t(green() * amount), std::uint8_t(blue() * amount), alpha());
  }
  
  inline constexpr color with_alpha(std::uint8_t alpha) const {
    return color(red(), green(), blue(), alpha);
  }

private:
  enum shift : std::uint32_t {
    shift_red = 24,
    shift_green = 16,
    shift_blue = 8,
    shift_alpha = 0,
  };

  enum bits : std::uint32_t {
    bits_red = 0xFF000000,
    bits_green = 0x00FF0000,
    bits_blue = 0x0000FF00,
    bits_alpha = 0x000000FF,
  };
  std::uint32_t _rgba = 0;
};
} // namespace fst::ui
