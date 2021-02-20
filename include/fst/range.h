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
#include "fst/traits.h"
#include <algorithm>

namespace fst {
template <typename T>
struct range {
  using value_type = T;
  T min;
  T max;
};

template <typename T>
inline constexpr range<T> make_range(T min, T max) {
  return range<T>{ min, max };
}

template <const auto& _Range>
struct clipped_value {
  using value_type = typename std::remove_cvref_t<decltype(_Range)>::value_type;

  clipped_value(value_type v)
      : value(std::clamp(v, _Range.min, _Range.max)) {}

  inline operator value_type() const noexcept { return value; }

  value_type value;
};

template <const auto& _Range>
struct assert_clipped_value {
  using value_type = typename std::remove_cvref_t<decltype(_Range)>::value_type;

#if __FST_HAS_DEBUG_ASSERT
  assert_clipped_value(value_type v) {
    fst_assert(v >= _Range.min && v <= _Range.max, "value out of range, should be inside [" + std::to_string(_Range.min) + ", " + std::to_string(_Range.max) + "].");
    value = std::clamp(v, _Range.min, _Range.max);
  }
#else
  assert_clipped_value(value_type v)
      : value(std::clamp(v, _Range.min, _Range.max)) {}
#endif // __FST_HAS_DEBUG_ASSERT

  inline operator value_type() const noexcept { return value; }

  value_type value;
};
} // namespace fst.
