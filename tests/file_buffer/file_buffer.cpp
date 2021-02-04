#include <gtest/gtest.h>
#include <string_view>
#include "fst/file_buffer.h"

namespace {
TEST(file_buffer, open) {
  fst::file_buffer fb;
  EXPECT_EQ(fb.open(FST_TEST_RESOURCES_DIRECTORY "/test.txt"), true);
  EXPECT_EQ(fb.str(), "Test");
}
} // namespace
