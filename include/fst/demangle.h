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
#include <typeinfo>

// clang-format off
#if __has_include(<cxxabi.h>)
  #include <cxxabi.h>
  #include <cstdio>
  #define FST_HAS_DEMANGLE 1
  namespace fst::config { inline constexpr bool has_demangle = true; }
#else
  #define FST_HAS_DEMANGLE 0
  namespace fst::config { inline constexpr bool has_demangle = false; }
#endif
// clang-format on

namespace fst {
#if FST_HAS_DEMANGLE
inline std::string demangle(const char* mangled_name) {
  std::size_t size;
  int status;

  // https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
  char* result = abi::__cxa_demangle(mangled_name, nullptr, &size, &status);

  if (status == 0) {
    std::string name(result);
    std::free(result);
    return name;
  }

  return mangled_name;
}

template <typename T>
inline std::string demangle() {
  return demangle(typeid(T).name());
}

#else // No demangle.

inline std::string demangle(const char* mangled_name) { return mangled_name; }

template <typename T>
inline std::string demangle() {
  return demangle(typeid(T).name());
}
#endif // FST_HAS_DEMANGLE.
} // namespace fst.
