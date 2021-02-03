#include <gtest/gtest.h>
#include <string>
#include "fst/scoped_singleton.h"

namespace {
class MySingleton {
public:
  std::string string_value;
};

using MyScopedSingleton = fst::scoped_singleton<MySingleton>;
using MySingletonInstance = MyScopedSingleton::shared_ptr_type;

struct MySingletonInitializer {
  inline static void init(const std::string& str) { MyScopedSingleton::get()->string_value = str; }
  inline static void init() { MyScopedSingleton::get()->string_value = "Peter"; }
};

using MySingletonRetainer = fst::scoped_singleton_retainer<MySingleton, MySingletonInitializer>;

TEST(scoped_singleton, simple_retain) {
  {
    MySingletonInstance instance = MyScopedSingleton::retain();
    instance->string_value = "Banana";

    EXPECT_EQ(MyScopedSingleton::get()->string_value, "Banana");
  }

  if constexpr (fst::config::has_assert) {
    EXPECT_DEATH({ MyScopedSingleton::get(); }, "");
  }
  else {
    EXPECT_EQ(MyScopedSingleton::get(), nullptr);
  }
}

TEST(scoped_singleton, double_retain) {
  MySingletonInstance instance2;

  {
    MySingletonInstance instance = MyScopedSingleton::retain();
    instance->string_value = "Banana";

    EXPECT_EQ(MyScopedSingleton::get()->string_value, "Banana");

    // Retain singleton in instance2.
    instance2 = MyScopedSingleton::retain();
  }

  EXPECT_NE(MyScopedSingleton::get(), nullptr);
  EXPECT_EQ(MyScopedSingleton::get()->string_value, "Banana");
}

TEST(scoped_singleton, unretained) {
  if constexpr (fst::config::has_assert) {
    EXPECT_DEATH({ MyScopedSingleton::get(); }, "");
  }
  else {
    EXPECT_EQ(MyScopedSingleton::get(), nullptr);
  }
}

TEST(scoped_singleton, retain_and_call) {
  MyScopedSingleton::retain()->string_value = "Banana";

  if constexpr (fst::config::has_assert) {
    EXPECT_DEATH({ MyScopedSingleton::get(); }, "");
  }
  else {
    EXPECT_EQ(MyScopedSingleton::get(), nullptr);
  }
}

TEST(scoped_singleton, initializer) {
  {
    MySingletonRetainer retainer("Jason");
    EXPECT_EQ(MyScopedSingleton::get()->string_value, "Jason");
  }

  if constexpr (fst::config::has_assert) {
    EXPECT_DEATH({ MyScopedSingleton::get(); }, "");
  }
  else {
    EXPECT_EQ(MyScopedSingleton::get(), nullptr);
  }
}

TEST(scoped_singleton, initializer_no_arg) {
  {
    MySingletonRetainer retainer;
    EXPECT_EQ(MyScopedSingleton::get()->string_value, "Peter");
  }

  if constexpr (fst::config::has_assert) {
    EXPECT_DEATH({ MyScopedSingleton::get(); }, "");
  }
  else {
    EXPECT_EQ(MyScopedSingleton::get(), nullptr);
  }
}

TEST(scoped_singleton, initializer_empty) {
  {
    fst::scoped_singleton_retainer<MySingleton> retainer;
    retainer->string_value = "Peter";
    EXPECT_EQ(MyScopedSingleton::get()->string_value, "Peter");
  }

  if constexpr (fst::config::has_assert) {
    EXPECT_DEATH({ MyScopedSingleton::get(); }, "");
  }
  else {
    EXPECT_EQ(MyScopedSingleton::get(), nullptr);
  }
}
} // namespace
