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
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>

// clang-format off
#if __FST_MSVC__
  #define __FST_DEBUG_BREAK __debugbreak()

#elif __FST_CLANG__
  #if __has_builtin(__builtin_debugtrap)
    #define __FST_DEBUG_BREAK __builtin_debugtrap()
  #elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__MACH__)
    #include <signal.h>
    #define __FST_DEBUG_BREAK raise(SIGTRAP)
  #else
    #define __FST_DEBUG_BREAK std::abort()
  #endif
#else
  #define __FST_DEBUG_BREAK std::abort()
#endif

#if defined(NDEBUG) && !defined(FST_USE_ASSERT_IN_RELEASE)
  #define fst_assert(Expr, Msg) ;
  #define __FST_HAS_DEBUG_ASSERT 0
  namespace fst::config { inline constexpr bool has_assert = false; }
#else
  #define __FST_HAS_DEBUG_ASSERT 1
  namespace fst::config { inline constexpr bool has_assert = true; }
  #define fst_assert(Expr, Msg) fst::assert_detail::custom_assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#endif

#define __FST_CALL_RELEASE_ASSERT fst::assert_detail::global_release_assert::call_assert
#define fst_release_assert(Expr, Msg) __FST_CALL_RELEASE_ASSERT(#Expr, Expr, __FILE__, __LINE__, Msg)
#undef __FST_CALL_RELEASE_ASSERT

// https://akrzemi1.wordpress.com/2017/05/18/asserts-in-constexpr-functions/
#if __FST_CLANG__ || __FST_GCC__
  #define fst_likely(EXPR) __builtin_expect(!!(EXPR), 1)
#else
  #define fst_likely(EXPR) (!!(EXPR))
#endif

#ifdef NDEBUG
  #define fst_cexpr_assert(CHECK) void(0)
#else
  #define fst_cexpr_assert(CHECK) ( fst_likely(CHECK) ?  void(0) : []{assert(!#CHECK);}() )
#endif

// clang-format on

  namespace fst {
  namespace assert_detail {
#if __FST_HAS_DEBUG_ASSERT
  inline void custom_assert(const char* expr_str, bool expr, const char* file, int line, const std::string& msg) {
    if (expr) {
      return;
    }

    std::cerr << "Assert failed:\t" << msg << "\n"
              << "Expected:\t" << expr_str << "\n"
              << "Source:\t\t" << file << ", line " << line << "\n";

    __FST_DEBUG_BREAK;
  }
#endif // __FST_HAS_DEBUG_ASSERT

  class global_release_assert {
  public:
    using callback_ptr = void (*)(const char*, int, const std::string&);

    inline static void set_callback(callback_ptr callback) { get_callback() = callback; }

    inline static void call_assert(
        const char* expr_str, bool expr, const char* file, int line, const std::string& msg) {
      if (expr) {
        return;
      }

      callback_ptr& callback = get_callback();
      if (callback) {
        callback(file, line, msg);
      }
      else {
        std::cerr << "Assert failed:\t" << msg << "\n"
                  << "Expected:\t" << expr_str << "\n"
                  << "Source:\t\t" << file << ", line " << line << "\n";
        std::abort();
      }
    }

  private:
    global_release_assert() = default;
    global_release_assert(const global_release_assert&) = delete;
    global_release_assert(global_release_assert&&) = delete;
    global_release_assert& operator=(const global_release_assert&) = delete;
    global_release_assert& operator=(global_release_assert&&) = delete;

    inline static callback_ptr& get_callback() {
      static callback_ptr callback = nullptr;
      return callback;
    }
  };
  } // namespace assert_detail.

inline void set_release_assert_callback(assert_detail::global_release_assert::callback_ptr callback) {
  assert_detail::global_release_assert::set_callback(callback);
}
} // namespace fst.
