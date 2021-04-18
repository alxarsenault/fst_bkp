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
#include "fst/common.h"
#include "fst/traits.h"

namespace fst {
template <class T>
inline constexpr std::conditional_t<!std::is_move_constructible<T>::value && std::is_copy_constructible<T>::value,
    const T&, T&&>
maybe_move(T& arg) noexcept {
  return std::move(arg);
}

// Fold expressions for c++ < 17.
// Calls your function with each of the provided variadic argument.
template <class Func, class... Args>
constexpr void fold(Func&& func, Args&&... args) {
  (func(args), ...);
}

namespace static_for_detail {
  template <class Func, size_t... I>
  constexpr void static_for(Func&& func, std::index_sequence<I...>) {
    return (func(std::integral_constant<size_t, I>{}), ...);
  }
} // namespace static_for_detail.

// Call a for loop at compile time.
// Your lambda is provided with an integral_constant.
// Accept it with auto, access the index with '::value'.
template <size_t N, class Func>
constexpr void static_for(Func&& func) {
  static_for_detail::static_for(std::forward<Func>(func), std::make_index_sequence<N>{});
}
} // namespace fst.
