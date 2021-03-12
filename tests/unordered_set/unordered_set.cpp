#include <gtest/gtest.h>

#include "fst/unordered_set.h"

namespace {
TEST(unordered_set, constructor) {
  fst::unordered_set<int> set;
  set.resize(5);

  EXPECT_EQ(5, set.maximum_size());
  EXPECT_EQ(0, set.size());

  set.insert(2);

  EXPECT_EQ(5, set.maximum_size());
  EXPECT_EQ(1, set.size());

  EXPECT_EQ(2, set[0]);

  std::span<const int> sp = set.content();
  for (int i : sp) {
    EXPECT_EQ(2, i);
  }

  // Inserting same value should do nothing.
  set.insert(2);
  EXPECT_EQ(5, set.maximum_size());
  EXPECT_EQ(1, set.size());

  set.insert(4);
  EXPECT_EQ(5, set.maximum_size());
  EXPECT_EQ(2, set.size());

  EXPECT_EQ(2, set[0]);
  EXPECT_EQ(4, set[1]);

  set.erase(2);
  EXPECT_EQ(5, set.maximum_size());
  EXPECT_EQ(1, set.size());

  EXPECT_EQ(4, set[0]);

  set.clear();
  EXPECT_EQ(5, set.maximum_size());
  EXPECT_EQ(0, set.size());
}
} // namespace
