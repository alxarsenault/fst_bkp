///
/// BSD 3-Clause License
///
/// Copyright (c) 2021, Alexandre Arsenault
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

// clang-format off
#pragma once
#include <cstddef>
#include <cstdint>

namespace fst::config {
  inline constexpr std::uint8_t version[3] = {1, 0, 0};
  enum class cpp_version_type {unknown, cpp_17, cpp_20};
  enum class bitness_type { unknown, b_32, b_64 };
  enum class compiler_type { unknown, clang, wasm, gcc, intel, mingw, msvc };
  enum class platform_type { unknown, android, bsd, ios, linux, macos, solaris, windows };

  //
  // C++ version.
  //
  #undef __FST_CPP_20__
  #undef __FST_CPP_17__

  // C++ 20.
  #if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
    #define __FST_CPP_20__ 1
    #define __FST_CPP_17__ 1
    inline constexpr cpp_version_type cpp_version = cpp_version_type::cpp_20;

  // C++ 17.
  #elif __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
    #define __FST_CPP_20__ 0
    #define __FST_CPP_17__ 1
    inline constexpr cpp_version_type cpp_version = cpp_version_type::cpp_17;

  #else
    #define __FST_CPP_20__ 0
    #define __FST_CPP_17__ 0
    #error "fst only support c++17 and up."
  #endif

  //
  // Bitness type.
  //
  #undef __FST_32_BIT__
  #undef __FST_64_BIT__

  // 32 bit.
  #if INTPTR_MAX == INT32_MAX
    #define __FST_32_BIT__ 1
    #define __FST_64_BIT__ 0
    inline constexpr bitness_type bitness = bitness_type::b_32;
    inline constexpr std::size_t bitness_byte_size = 4;

  // 64 bit.
  #elif INTPTR_MAX == INT64_MAX
    #define __FST_32_BIT__ 0
    #define __FST_64_BIT__ 1
    inline constexpr bitness_type bitness = bitness_type::b_64;
    inline constexpr std::size_t bitness_byte_size = 8;

  // Unknown.
  #else
    #define __FST_32_BIT__ 0
    #define __FST_64_BIT__ 0
    inline constexpr bitness_type bitness = bitness_type::unknown;
    inline constexpr std::size_t bitness_byte_size = 0;
    #error "fst only support 32 and 64 bit architecture."
  #endif

  //
  // Compiler type.
  //
  #undef __FST_CLANG__
  #undef __FST_GCC__
  #undef __FST_INTEL__
  #undef __FST_MINGW__
  #undef __FST_MSVC__
  #undef __FST_WASM__

  // Clang.
  #if defined(__clang__)
    #define __FST_CLANG__ 1
    #define __FST_GCC__   0
    #define __FST_INTEL__ 0
    #define __FST_MINGW__ 0
    #define __FST_MSVC__  0
    #define __FST_WASM__  0
    inline constexpr compiler_type compiler = compiler_type::clang;

  // GCC.
  #elif defined(__GNUC__) || defined(__GNUG__)
    #define __FST_CLANG__ 0
    #define __FST_GCC__   1
    #define __FST_INTEL__ 0
    #define __FST_MINGW__ 0
    #define __FST_MSVC__  0
    #define __FST_WASM__  0
    inline constexpr compiler_type compiler = compiler_type::gcc;

  // Intel.
  #elif (defined(SYCL_LANGUAGE_VERSION) && defined (__INTEL_LLVM_COMPILER)) || defined(__INTEL_COMPILER)
    #define __FST_CLANG__ 0
    #define __FST_GCC__   0
    #define __FST_INTEL__ 1
    #define __FST_MINGW__ 0
    #define __FST_MSVC__  0
    #define __FST_WASM__  0
    inline constexpr compiler_type compiler = compiler_type::intel;

  // MinGW.
  #elif defined(__MINGW32__) || defined(__MINGW64__)
    #define __FST_CLANG__ 0
    #define __FST_GCC__   0
    #define __FST_INTEL__ 0
    #define __FST_MINGW__ 1
    #define __FST_MSVC__  0
    #define __FST_WASM__  0
    inline constexpr compiler_type compiler = compiler_type::mingw;

  // Microsoft visual studio.
  #elif defined(_MSC_VER)
    #define __FST_CLANG__ 0
    #define __FST_GCC__   0
    #define __FST_INTEL__ 0
    #define __FST_MINGW__ 0
    #define __FST_MSVC__  1
    #define __FST_WASM__  0
    inline constexpr compiler_type compiler = compiler_type::msvc;

  // Web assembly.
  #elif defined(__EMSCRIPTEN__)
    #define __FST_CLANG__ 0
    #define __FST_GCC__   0
    #define __FST_INTEL__ 0
    #define __FST_MINGW__ 0
    #define __FST_MSVC__  0
    #define __FST_WASM__  1
    inline constexpr compiler_type compiler = compiler_type::wasm;

  // Unknown compiler.
  #else
    #define __FST_CLANG__ 0
    #define __FST_GCC__   0
    #define __FST_INTEL__ 0
    #define __FST_MINGW__ 0
    #define __FST_MSVC__  0
    #define __FST_WASM__  0
    inline constexpr compiler_type compiler = compiler_type::unknown;
    #error "fst unsupported compiler."
  #endif

  //
  // Platform type.
  //
  #undef __FST_ANDROID__
  #undef __FST_BSD__
  #undef __FST_IOS__
  #undef __FST_LINUX__
  #undef __FST_MACOS__
  #undef __FST_SOLARIS__
  #undef __FST_WINDOWS__

  // Android.
  #if defined(__ANDROID__)
    #define __FST_ANDROID__ 1
    #define __FST_BSD__     0
    #define __FST_IOS__     0
    #define __FST_LINUX__   0
    #define __FST_MACOS__   0
    #define __FST_SOLARIS__ 0
    #define __FST_WINDOWS__ 0
    inline constexpr platform_type platform = platform_type::android;

  // BSD.
  #elif defined(BSD) || defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #define __FST_ANDROID__ 0
    #define __FST_BSD__     1
    #define __FST_IOS__     0
    #define __FST_LINUX__   0
    #define __FST_MACOS__   0
    #define __FST_SOLARIS__ 0
    #define __FST_WINDOWS__ 0
    inline constexpr platform_type platform = platform_type::bsd;

  // Linux.
  #elif defined(__linux__) || defined(__linux) || defined(linux)
    #define __FST_ANDROID__ 0
    #define __FST_BSD__     1
    #define __FST_IOS__     0
    #define __FST_LINUX__   0
    #define __FST_MACOS__   0
    #define __FST_SOLARIS__ 0
    #define __FST_WINDOWS__ 0
    inline constexpr platform_type platform = platform_type::linux;

  // Apple macos or ios.
  #elif __APPLE__
    // Apple.
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
      // ios.
      #define __FST_ANDROID__ 0
      #define __FST_BSD__     0
      #define __FST_IOS__     1
      #define __FST_LINUX__   0
      #define __FST_MACOS__   0
      #define __FST_SOLARIS__ 0
      #define __FST_WINDOWS__ 0
      inline constexpr platform_type platform = platform_type::ios;

    #elif TARGET_OS_MAC
      // Mac OS.
      #define __FST_ANDROID__ 0
      #define __FST_BSD__     0
      #define __FST_IOS__     0
      #define __FST_LINUX__   0
      #define __FST_MACOS__   1
      #define __FST_SOLARIS__ 0
      #define __FST_WINDOWS__ 0
      inline constexpr platform_type platform = platform_type::macos;

    #else
      // Unknown apple platform.
      #define __FST_ANDROID__ 0
      #define __FST_BSD__     0
      #define __FST_IOS__     0
      #define __FST_LINUX__   0
      #define __FST_MACOS__   0
      #define __FST_SOLARIS__ 0
      #define __FST_WINDOWS__ 0
      inline constexpr platform_type platform = platform_type::unknown;
      #error "fst unsupported platform."
    #endif

  // Solaris.
  #elif defined(__sun) && defined(__SVR4)
    #define __FST_ANDROID__ 0
    #define __FST_BSD__     0
    #define __FST_IOS__     0
    #define __FST_LINUX__   0
    #define __FST_MACOS__   0
    #define __FST_SOLARIS__ 1
    #define __FST_WINDOWS__ 0
    inline constexpr platform_type platform = platform_type::solaris;

  // Windows.
  #elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(_WIN64)
    #define __FST_ANDROID__ 0
    #define __FST_BSD__     0
    #define __FST_IOS__     0
    #define __FST_LINUX__   0
    #define __FST_MACOS__   0
    #define __FST_SOLARIS__ 0
    #define __FST_WINDOWS__ 1
    inline constexpr platform_type platform = platform_type::windows;

  // Unknown platform.
  #else
    #define __FST_ANDROID__ 0
    #define __FST_BSD__     0
    #define __FST_IOS__     0
    #define __FST_LINUX__   0
    #define __FST_MACOS__   0
    #define __FST_SOLARIS__ 0
    #define __FST_WINDOWS__ 0
    inline constexpr platform_type platform = platform_type::unknown;
    #error "fst unsupported platform."
  #endif

  //
  // unistd.h
  //
  #undef __FST_UNISTD__

  #if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
    #define __FST_UNISTD__ 1
    inline constexpr bool has_unistd = true;
  #else
    #define __FST_UNISTD__ 0
    inline constexpr bool has_unistd = false;
  #endif

  //
  // Exceptions.
  //
  #if defined(__FST_NO_EXCEPTIONS__) || !(defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND))
    inline constexpr bool has_exceptions = false;

    #if !defined(__FST_NO_EXCEPTIONS__)
      #define __FST_NO_EXCEPTIONS__
    #endif
  #else
    inline constexpr bool has_exceptions = true;
  #endif // __FST_NO_EXCEPTIONS__

} // namespace fst::config.

#if __FST_MSVC__ == 1
  #define fst_inline __forceinline
#elif __FST_CLANG__ || __FST_GCC__ || __FST_INTEL__
  #define fst_inline __attribute__((always_inline))
#else
  #define fst_inline inline
#endif

// clang-format on

// "https://web.archive.org/web/20140625123925/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system"
