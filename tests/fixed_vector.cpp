#include <gtest/gtest.h>

#include "fst/fixed_vector.h"

namespace {
TEST(fixed_vector, constructor) {
  fst::fixed_vector<float, 2> a;
  EXPECT_EQ(a.size(), 0);

  a.push_back(3);
  a.push_back(4);
  EXPECT_EQ(a.size(), 2);
  EXPECT_EQ(a[0], 3);
  EXPECT_EQ(a[1], 4);

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

TEST(fixed_vector, push) {
  fst::fixed_vector<float, 2> a;
  EXPECT_EQ(a.size(), 0);

  a.push_back(3);
  a.push_back(4);
  EXPECT_EQ(a.size(), 2);
  EXPECT_EQ(a[0], 3);
  EXPECT_EQ(a[1], 4);

  using pair_type = std::pair<int, int>;
  fst::fixed_vector<pair_type, 2> b;
  b.push_back(pair_type(3, 4));
  b.push_back(std::move(pair_type(5, 6)));
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(b[0], pair_type(3, 4));
  EXPECT_EQ(b[1], pair_type(5, 6));

  fst::fixed_vector<pair_type, 2> c = b;
  EXPECT_EQ(c.size(), 2);
  EXPECT_EQ(c[0], pair_type(3, 4));
  EXPECT_EQ(c[1], pair_type(5, 6));

  fst::fixed_vector<pair_type, 2> d = std::move(c);
  EXPECT_EQ(c.size(), 0);
  EXPECT_EQ(d.size(), 2);
  EXPECT_EQ(d[0], pair_type(3, 4));
  EXPECT_EQ(d[1], pair_type(5, 6));
}
} // namespace
