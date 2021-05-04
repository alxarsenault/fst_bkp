#include <gtest/gtest.h>
#include <string_view>
#include <filesystem>
#include "fst/mapped_file.h"
#include "fst/print.h"

namespace {
TEST(mapped_file, open) {
  fst::mapped_file fb;
  std::filesystem::path filepath = FST_TEST_RESOURCES_DIRECTORY "/test.txt";

  fst::print(FST_TEST_RESOURCES_DIRECTORY "/test.txt");
  EXPECT_EQ(fb.open(FST_TEST_RESOURCES_DIRECTORY "/test.txt"), true);
  EXPECT_EQ(fb.str(), "Test");
}
} // namespace
