#include <gtest/gtest.h>

#include "fst/int24_t.h"

namespace {
TEST(int24_t, tests) {
  {
    fst::int24_t i = 0;
    EXPECT_EQ(i, 0);

    int j = i;
    EXPECT_EQ(j, 0);
  }

  {
    fst::int24_t i = 32;
    EXPECT_EQ(i, 32);

    int j = i;
    EXPECT_EQ(j, 32);
  }

  {
    fst::int24_t i = -32;
    EXPECT_EQ(i, -32);

    int j = i;
    EXPECT_EQ(j, -32);
  }
}
} // namespace
