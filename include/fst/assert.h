#pragma once

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>

#ifdef NDEBUG
#define fst_assert(Expr, Msg) ;
#else
#define fst_assert(Expr, Msg) fst::assert_detail::custom_assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#endif

#define fst_error(Msg) fst_assert(false, Msg)

namespace fst {
namespace config {
#ifdef NDEBUG
  inline constexpr bool has_asser = false;
#else
  inline constexpr bool has_assert = true;
#endif
} // namespace config.

namespace assert_detail {
#ifndef NDEBUG
  inline void custom_assert(const char* expr_str, bool expr, const char* file, int line, const std::string& msg) {
    if (expr) {
      return;
    }

    std::cerr << "Assert failed:\t" << msg << "\n"
              << "Expected:\t" << expr_str << "\n"
              << "Source:\t\t" << file << ", line " << line << "\n";
    std::abort();
  }
#endif // NDEBUG
} // namespace assert_detail.
} // namespace fst.
