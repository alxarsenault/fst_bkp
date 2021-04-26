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

#pragma once
#include "fst/common.h"

// clang-format off
#if __FST_UNISTD__
  #include <unistd.h>
#endif // __FST_UNISTD__

#if __FST_MACOS__
  #include <sys/sysctl.h>
#elif __FST_WINDOWS__
  #include <stdlib.h>
  #include <windows.h>
  #include <sysinfoapi.h>
#elif __FST_LINUX__
  #include <stdio.h>
#endif
// clang-format on

namespace fst::memory {
namespace detail {
//
// get_page_size.
//
#if __FST_UNISTD__
inline std::size_t get_page_size() {
  long pagesize = sysconf(_SC_PAGE_SIZE);
  return pagesize >= 0 ? (std::size_t)pagesize : 0;
}

#elif __FST_WINDOWS__
  inline std::size_t get_page_size() {
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    return sys_info.dwPageSize >= 0 ? (std::size_t)sys_info.dwPageSize : 0;
  }

#else
inline std::size_t get_page_size() {
  return 0;
}
#endif

//
// get_cache_size.
//
// https://stackoverflow.com/questions/794632/programmatically-get-the-cache-line-size

#if __FST_MACOS__
inline std::size_t get_cache_size() {
  size_t line_size = 0;
  size_t sizeof_line_size = sizeof(line_size);
  sysctlbyname("hw.cachelinesize", &line_size, &sizeof_line_size, 0, 0);
  return line_size;
}

#elif __FST_WINDOWS__
  inline std::size_t get_cache_size() {
    size_t line_size = 0;
    DWORD buffer_size = 0;
    DWORD i = 0;
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION * buffer = 0;

    GetLogicalProcessorInformation(0, &buffer_size);
    buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
    GetLogicalProcessorInformation(&buffer[0], &buffer_size);

    for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
        if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) {
            line_size = buffer[i].Cache.LineSize;
            break;
        }
    }

    free(buffer);
    return line_size;
  }

#elif __FST_LINUX__
inline std::size_t get_cache_size() {
  FILE* p = 0;
  p = fopen("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size", "r");
  unsigned int i = 0;
  if (p) {
    fscanf(p, "%d", &i);
    fclose(p);
  }
  return i;
}

#else
#warning "Unsupported platform."
  inline std::size_t get_cache_size() { return 0; }
#endif
} // namespace detail.

inline std::size_t get_page_size() {
  static std::size_t size = detail::get_page_size();
  return size;
}

inline std::size_t get_cache_size() {
  static std::size_t size = detail::get_cache_size();
  return size;
}
} // namespace fst::memory
