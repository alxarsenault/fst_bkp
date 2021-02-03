#include <gtest/gtest.h>

#include "fst/fixed_vector.h"

namespace {
TEST(fixed_vector, constructor) {
  fst::fixed_vector<float, 2> a;
  EXPECT_EQ(a.size(), 0);
//
//  // Move a to b, b should now be empty.
//  fst::unordered_array<int, 2> b(std::move(a));
//  EXPECT_EQ(b.size(), 2);
//  EXPECT_EQ(a.size(), 0);
//
//  // Copy b to c, b remains unchanged.
//  fst::unordered_array<int, 2> c = b;
//  EXPECT_EQ(c.size(), 2);
//  EXPECT_EQ(b.size(), 2);
//
//  a = { 3 };
//  EXPECT_EQ(a.size(), 1);
//  EXPECT_EQ(a[0], 3);
//
//  a = { 4, 5 };
//  EXPECT_EQ(a.size(), 2);
//  EXPECT_EQ(a[0], 4);
//  EXPECT_EQ(a[1], 5);
}
} // namespace
