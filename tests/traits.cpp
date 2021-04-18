#include <gtest/gtest.h>
#include "fst/traits.h"

namespace {
TEST(dtraits, is_fastest_without_const_reference) {
  // Fundamental type.
  {
    bool value = fst::is_faster_without_const_reference<float>::value;
    EXPECT_EQ(value, true);
  }

  // Fundamental type.
  {
    bool value = fst::is_faster_without_const_reference<long double>::value;
    EXPECT_EQ(value, true);
  }

  // Trivially copy constructible.
  {
    struct type {
      float a, b;
    };

    bool value = fst::is_faster_without_const_reference<type>::value;
    EXPECT_EQ(value, sizeof(type) <= fst::config::bitness_byte_size);
  }

  // Trivially copy constructible but too big (for 64 bits).
  {
    struct type {
      double a;
      float b;
    };
    bool value = fst::is_faster_without_const_reference<type>::value;
    EXPECT_EQ(value, sizeof(type) <= fst::config::bitness_byte_size);
  }

  // Trivially copy constructible, too big but with tag.
  {
    struct type {
      double a;
      float b;
      using is_faster_without_const_reference = fst::faster_without_const_reference_tag;
    };

    bool value = fst::is_faster_without_const_reference<type>::value;
    EXPECT_EQ(value, true);
  }

  // Not trivially copy constructible.
  {
    struct type {
      type() {}
      type(const type&) {}
      float a, b;
    };

    bool value = fst::is_faster_without_const_reference<type>::value;
    EXPECT_EQ(value, false);
  }

  // Not trivially copy constructible.
  {
    struct type {
      type() {}
      type(const type&) = delete;
      float a, b;
    };

    bool value = fst::is_faster_without_const_reference<type>::value;
    EXPECT_EQ(value, false);
  }

  // Trivially copy constructible but not copy assignable.
  {
    struct type {
      type() {}
      type& operator=(const type&) = delete;
      float a, b;
    };

    bool value = fst::is_faster_without_const_reference<type>::value;
    EXPECT_EQ(value, true);
  }
}
} // namespace
