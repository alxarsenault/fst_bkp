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
#include "fst/ascii.h"
#include <string_view>

namespace fst::string {

inline constexpr bool is_upper_case(std::string_view s) {
  for (std::size_t i = 0; i < s.size(); i++) {
    if (fst::is_letter(s[i]) && !fst::is_upper_case_letter(s[i])) {
      return false;
    }
  }

  return true;
}

inline constexpr bool is_lower_case(std::string_view s) {
  for (std::size_t i = 0; i < s.size(); i++) {
    if (fst::is_letter(s[i]) && !fst::is_lower_case_letter(s[i])) {
      return false;
    }
  }

  return true;
}

inline constexpr bool is_alphanumeric(std::string_view s) {
  for (std::size_t i = 0; i < s.size(); i++) {
    if (!fst::is_alphanumeric(s[i])) {
      return false;
    }
  }

  return true;
}

inline constexpr bool is_alphanumeric_with_spaces(std::string_view s) {
  for (std::size_t i = 0; i < s.size(); i++) {
    if (!(fst::is_alphanumeric(s[i]) || fst::is_space(s[i]))) {
      return false;
    }
  }

  return true;
}

inline constexpr bool has_leading_spaces(std::string_view s) { return s.empty() ? false : fst::is_space(s[0]); }

inline constexpr bool has_trailing_spaces(std::string_view s) { return s.empty() ? false : fst::is_space(s.back()); }

inline constexpr bool has_end_of_line(std::string_view s) {
  for (std::size_t i = 0; i < s.size(); i++) {
    if (fst::is_end_of_line(s[i])) {
      return true;
    }
  }

  return false;
}

inline constexpr std::string_view strip_leading_spaces(std::string_view s) {
  const std::size_t b = s.find_first_not_of(' ');
  if (b == std::string_view::npos) {
    return std::string_view();
  }

  return s.substr(b);
}

inline constexpr std::string_view strip_trailing_spaces(std::string_view s) {
  const std::size_t b = s.find_last_not_of(' ');
  if (b == std::string_view::npos) {
    return std::string_view();
  }

  return s.substr(0, b + 1);
}

inline constexpr std::string_view strip_leading_and_trailing_spaces(std::string_view s) {
  return strip_trailing_spaces(strip_leading_spaces(s));
}

inline constexpr bool is_signed_integer(std::string_view s) {
  if (s.empty()) {
    return false;
  }

  std::size_t start_index = 0;

  if (s[0] == '-') {
    if (s.size() == 1) {
      return false;
    }

    start_index = 1;
  }

  for (std::size_t i = start_index; i < s.size(); i++) {
    if (!fst::is_digit(s[i])) {
      return false;
    }
  }

  return true;
}

inline constexpr bool is_unsigned_integer(std::string_view s) {
  if (s.empty()) {
    return false;
  }

  for (std::size_t i = 0; i < s.size(); i++) {
    if (!fst::is_digit(s[i])) {
      return false;
    }
  }

  return true;
}

inline constexpr bool is_floating_point(std::string_view s) {
  if (s.empty()) {
    return false;
  }

  std::size_t start_index = 0;

  if (s[0] == '-') {
    if (s.size() == 1) {
      return false;
    }

    start_index = 1;
  }

  // Find point index.
  const std::size_t point_index = s.find_first_of('.');

  if (point_index == std::string_view::npos) {
    for (std::size_t i = start_index; i < s.size(); i++) {
      if (!fst::is_digit(s[i])) {
        return false;
      }
    }

    return true;
  }

  // Point exist.

  // Check if point is at the beginning or at the end.
  if ((start_index == point_index) || (point_index == s.size() - 1)) {
    return false;
  }

  // Check left side.
  for (std::size_t i = start_index; i < point_index; i++) {
    if (!fst::is_digit(s[i])) {
      return false;
    }
  }

  // Check right side.
  for (std::size_t i = point_index + 1; i < s.size(); i++) {
    if (!fst::is_digit(s[i])) {
      return false;
    }
  }

  return true;
}

inline constexpr bool is_convertible_to_signed_integer(std::string_view s) {
  return is_signed_integer(strip_leading_and_trailing_spaces(s));
}

inline constexpr bool is_convertible_to_unsigned_integer(std::string_view s) {
  return is_unsigned_integer(strip_leading_and_trailing_spaces(s));
}

inline constexpr bool is_convertible_to_floating_point(std::string_view s) {
  return is_floating_point(strip_leading_and_trailing_spaces(s));
}

// template <std::size_t N>
// bool small_string<N>::is_float() const
//{
//	small_string<N> tmp_str(*this);
//	tmp_str.strip_leading_spaces_and_tabs();
//	tmp_str.strip_trailing_spaces_and_tabs();
//
//	unsigned int start_index = 0;
//
//	if (tmp_str[0] == '-') {
//		if (tmp_str.size() == 1) {
//			return false;
//		}
//
//		start_index = 1;
//	}
//
//	// Find point index.
//	long point_index = tmp_str.index_of_first([](char c) { return c == '.'; });
//
//	if (point_index == -1) {
//		for (unsigned int i = start_index; i < tmp_str.size(); i++) {
//			if (!fst::ascii::is_digit(tmp_str[i])) {
//				return false;
//			}
//		}
//
//		return true;
//	}
//
//	// Point exist.
//
//	// Check if point is at the beginning.
//	if (start_index == point_index) {
//		return false;
//	}
//
//	// Check if point is at the end.
//	if (point_index == (long)tmp_str.size() - 1) {
//		return false;
//	}
//
//	// Check left side.
//	for (unsigned int i = start_index; i < point_index; i++) {
//		if (!fst::ascii::is_digit(tmp_str[i])) {
//			return false;
//		}
//	}
//
//	// Check right side.
//	for (unsigned int i = point_index + 1; i < tmp_str.size(); i++) {
//		if (!fst::ascii::is_digit(tmp_str[i])) {
//			return false;
//		}
//	}
//
//	return true;
//}

// template <std::size_t N>
// bool small_string<N>::is_int() const
//{
//	small_string<N> tmp_str(*this);
//	tmp_str.strip_leading_spaces_and_tabs();
//	tmp_str.strip_trailing_spaces_and_tabs();
//
//	unsigned int start_index = 0;
//
//	if (tmp_str[0] == '-') {
//		if (tmp_str.size() == 1) {
//			return false;
//		}
//
//		start_index = 1;
//	}
//
//	for (unsigned int i = start_index; i < tmp_str.size(); i++) {
//		if (!fst::ascii::is_digit(tmp_str[i])) {
//			return false;
//		}
//	}
//
//	return true;
//}

// template <std::string_view const&... Strs>
// struct join {
//  // Helper to get a string literal from a std::array
//  template <std::size_t N, std::array<char, N> const& S, typename>
//  struct to_char_array;
//
//  template <std::size_t N, std::array<char, N> const& S, std::size_t... I>
//  struct to_char_array<N, S, std::index_sequence<I...>> {
//    static constexpr const char value[]{ S[I]..., 0 };
//  };
//
//  // Join all strings into a single std::array of chars
//  static constexpr auto impl() noexcept {
//    constexpr std::size_t len = (Strs.size() + ... + 0);
//    std::array<char, len + 1> arr{};
//    auto append = [i = 0, &arr](auto const& s) mutable {
//      for (auto c : s)
//        arr[i++] = c;
//    };
//    (append(Strs), ...);
//    arr[len] = 0;
//    return arr;
//  }
//  // Give the joined string static storage
//  static constexpr auto arr = impl();
//  // Convert to a string literal, then view as a std::string_view
//  static constexpr std::string_view value = to_char_array<arr.size(), arr,
//  std::make_index_sequence<arr.size()>>::value;
//};
//
//// Helper to get the value out
// template <std::string_view const&... Strs>
// static constexpr auto join_v = join<Strs...>::value;
} // namespace fst::string
