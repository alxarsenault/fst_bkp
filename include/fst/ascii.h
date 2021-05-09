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
#include <cstddef>

namespace fst {
inline constexpr char distance_between_lower_and_upper_case() { return 'a' - 'A'; }

inline constexpr bool is_char(char c) { return c >= 0 && c <= 127; }

inline constexpr bool is_null(char c) { return c == 0; }

inline constexpr bool is_space(char c) { return c == ' '; }

inline constexpr bool is_tab(char c) { return c == '\t'; }

inline constexpr bool is_space_or_tab(char c) { return is_space(c) || is_tab(c); }

inline constexpr bool is_digit(char c) { return c >= '0' && c <= '9'; }

inline constexpr unsigned int to_digit(char c) { return c - '0'; }

inline constexpr bool is_letter(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }

inline constexpr bool is_lower_case_letter(char c) { return c >= 'a' && c <= 'z'; }

inline constexpr bool is_upper_case_letter(char c) { return c >= 'A' && c <= 'Z'; }

inline constexpr bool is_alphanumeric(char c) { return is_digit(c) || is_letter(c); }

inline constexpr bool is_letter_or_underscore(char c) { return is_letter(c) || c == '_'; }

inline constexpr bool is_alphanumeric_or_underscore(char c) { return is_alphanumeric(c) || c == '_'; }

inline constexpr bool is_control(char c) { return (c >= 0 && c <= 31) || c == 127; }

inline constexpr bool is_end_of_line(char c) { return c == '\n' || c == '\r'; }

inline constexpr bool is_special(char c) {
  // ! " # $ % & ' ( ) * + - . / : ; < = > ? @ [ \ ] ^ _ ` { | } ~
  return (c >= 32 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126);
}

inline constexpr char to_upper_case(char c) {
  return is_lower_case_letter(c) ? (c - distance_between_lower_and_upper_case()) : c;
}

inline constexpr char to_lower_case(char c) {
  return is_upper_case_letter(c) ? (c + distance_between_lower_and_upper_case()) : c;
}
} // namespace fst
