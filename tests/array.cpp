#include <gtest/gtest.h>

#include <array>
#include "fst/array.h"

namespace {
TEST(array, constructor) {
  fst::array<float, 3> a = { 0, 1, 2 };
  fst::array<float, 3> b = { 3, 4, 5 };
  EXPECT_EQ(a[0], 0);
  EXPECT_EQ(a[1], 1);
  EXPECT_EQ(a[2], 2);

  std::array<float, 3> c = { 6, 7, 8 };
  //  a = c;

  b = a;

  struct banana {
    float data[3];
  };

  banana aa = { 0, 1, 2 };
  banana bb = { 3, 4, 5 };
  //  float aa[3] = {0, 1, 2};
  //  float bb[3] = {3, 4, 5};

  bb = aa;
  EXPECT_EQ(bb.data[0], 0);
  EXPECT_EQ(bb.data[1], 1);
  EXPECT_EQ(bb.data[2], 2);
}

} // namespace
