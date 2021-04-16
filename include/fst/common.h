#pragma once

#include <cstddef>

#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
#undef __FST_CPP_20__
#undef __FST_CPP_17__
#define __FST_CPP_20__ 1
#define __FST_CPP_17__ 1
#elif __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#undef __FST_CPP_20__
#undef __FST_CPP_17__
#define __FST_CPP_17__ 1
#else
#error "fst only support c++17 and up."
#endif

#ifndef __FST_NO_EXCEPTIONS__
// Attempt to discover whether we're being compiled with exception support.
#if !(defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND))
#define __FST_NO_EXCEPTIONS__
#endif
#endif
