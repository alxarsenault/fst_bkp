#include <gtest/gtest.h>

#include "fst/print.h"
#include "fst/system.h"
#include "pwd.h"

namespace {
TEST(system_path, constructor) {
  EXPECT_EQ(false, false);

  std::filesystem::path p = fst::get_user_app_data_directory();
  fst::print("KKJLJKLJK", p);
}

} // namespace
