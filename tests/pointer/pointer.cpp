#include <gtest/gtest.h>

#include "fst/pointer.h"

namespace {
TEST(pointer, constructor) {
  fst::optional_owned_ptr<int> p(new int(32), true);
  EXPECT_EQ(p.is_valid(), true);
  EXPECT_EQ(p.is_owned(), true);

  EXPECT_EQ(*p.get(), 32);
  EXPECT_EQ(*p, 32);

  *p = 34;

  EXPECT_EQ(*p.get(), 34);
  EXPECT_EQ(*p, 34);

  p.reset();
  EXPECT_EQ(p.is_valid(), false);
  EXPECT_EQ(p.is_owned(), false);
}

TEST(pointer, copy) {
  fst::optional_owned_ptr<int> p(new int(32), true);
  EXPECT_EQ(p.is_valid(), true);
  EXPECT_EQ(p.is_owned(), true);

  EXPECT_EQ(*p.get(), 32);
  EXPECT_EQ(*p, 32);

  fst::optional_owned_ptr<int> p2 = std::move(p);
  EXPECT_EQ(p2.is_valid(), true);
  EXPECT_EQ(p2.is_owned(), true);
  EXPECT_EQ(p.is_valid(), false);
  EXPECT_EQ(p.is_owned(), false);

  p2 = nullptr;
  EXPECT_EQ(p2.is_valid(), false);
  EXPECT_EQ(p2.is_owned(), false);
}
} // namespace
