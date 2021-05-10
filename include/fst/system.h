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
#include "fst/assert.h"
#include <filesystem>

#if __FST_MACOS__
#include <pwd.h>
#elif __FST_WINDOWS__
#include <shlobj_core.h>
#endif

namespace fst {
#if __FST_MACOS__
namespace system_detail {
  inline std::filesystem::path get_home_directory() {
    passwd* p = getpwuid(getuid());
    fst_assert(p != nullptr, "Can't get pwuid.");

    return p ? std::filesystem::path(p->pw_dir) : "";
  }

} // namespace system_detail.
inline std::filesystem::path get_home_directory() {
  static std::filesystem::path home_dir = system_detail::get_home_directory();
  return home_dir;
  ;
}

inline std::filesystem::path get_user_app_data_directory() {
  return get_home_directory() / "Library/Application Support";
}

inline std::filesystem::path get_common_app_data_directory() { return "/Library"; }

inline std::filesystem::path get_global_app_directory() { return "/Applications"; }

#elif __FST_WINDOWS__
namespace system_detail {
  inline std::filesystem::path get_window_special_folder_path(int type) {
    WCHAR path[MAX_PATH + 256];
    if (SHGetSpecialFolderPath(nullptr, path, type, FALSE)) {
      return std::filesystem::path(path);
    }

    return "";
  }
} // namespace system_detail.

inline std::filesystem::path get_user_app_data_directory() {
  return system_detail::get_window_special_folder_path(CSIDL_APPDATA);
}

inline std::filesystem::path get_common_app_data_directory() {
  return system_detail::get_window_special_folder_path(CSIDL_COMMON_APPDATA);
}

inline std::filesystem::path get_global_app_directory() {
  return syspath_detail::get_window_special_folder_path(CSIDL_PROGRAM_FILES);
}
#else

#warning "fst::system_path not implemented for this platform."

inline std::filesystem::path get_user_app_data_directory() { return ""; }

inline std::filesystem::path get_common_app_data_directory() { return ""; }

inline std::filesystem::path get_global_app_directory() { return ""; }
#endif // __FST_MACOS__
} // namespace fst.
