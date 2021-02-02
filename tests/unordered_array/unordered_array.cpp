#include <gtest/gtest.h>

#include "fst/unordered_array.h"

namespace {
TEST(unordered_array, constructor) {
  fst::unordered_array<int, 2> a = { 1, 2 };
  EXPECT_EQ(a.size(), 2);

  // Move a to b, b should now be empty.
  fst::unordered_array<int, 2> b(std::move(a));
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(a.size(), 0);

  // Copy b to c, b remains unchanged.
  fst::unordered_array<int, 2> c = b;
  EXPECT_EQ(c.size(), 2);
  EXPECT_EQ(b.size(), 2);

  a = { 3 };
  EXPECT_EQ(a.size(), 1);
  EXPECT_EQ(a[0], 3);

  a = { 4, 5 };
  EXPECT_EQ(a.size(), 2);
  EXPECT_EQ(a[0], 4);
  EXPECT_EQ(a[1], 5);
}

TEST(unordered_array, push_back) {
  fst::unordered_array<int, 2> buffer_2;
  // Make sure the array is empty.
  EXPECT_EQ(buffer_2.size(), 0);
  EXPECT_EQ(buffer_2.empty(), true);

  buffer_2.push_back(1);
  buffer_2.push_back(2);
  EXPECT_EQ(buffer_2.size(), 2);
  EXPECT_EQ(buffer_2[0], 1);
  EXPECT_EQ(buffer_2[1], 2);

  // Adding another element should assert.
  if constexpr (fst::config::has_assert) {
    EXPECT_DEATH({ buffer_2.push_back(3); }, "");
  }

  // Content should still be valid with only two elements.
  EXPECT_EQ(buffer_2.size(), 2);
  EXPECT_EQ(buffer_2[0], 1);
  EXPECT_EQ(buffer_2[1], 2);

  // Remove last element.
  buffer_2.pop_back();
  EXPECT_EQ(buffer_2.size(), 1);
  EXPECT_EQ(buffer_2[0], 1);

  // Remove last element and make sure the array is empty.
  buffer_2.pop_back();
  EXPECT_EQ(buffer_2.size(), 0);
  EXPECT_EQ(buffer_2.empty(), true);

  // Removing another element should assert.
  if constexpr (fst::config::has_assert) {
    EXPECT_DEATH({ buffer_2.pop_back(); }, "");
  }

  // Content should still be valid with no element.
  EXPECT_EQ(buffer_2.size(), 0);
  EXPECT_EQ(buffer_2.empty(), true);

  buffer_2.push_back(1);
  buffer_2.push_back(2);
  EXPECT_EQ(buffer_2.size(), 2);
  EXPECT_EQ(buffer_2[0], 1);
  EXPECT_EQ(buffer_2[1], 2);
}
} // namespace
