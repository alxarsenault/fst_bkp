#include <gtest/gtest.h>
#include <string_view>
#include "fst/mapped_file.h"

namespace {
TEST(mapped_file, open) {
  fst::mapped_file fb;
  EXPECT_EQ(fb.open(FST_TEST_RESOURCES_DIRECTORY "/test.txt"), true);
  EXPECT_EQ(fb.str(), "Test");
}
} // namespace
