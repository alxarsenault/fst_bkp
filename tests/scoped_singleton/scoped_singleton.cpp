#include <gtest/gtest.h>
#include <string>
#include "fst/scoped_singleton.h"

namespace {
class MySingleton {
public:
  MySingleton() = default;
  MySingleton(const std::string& str)
      : string_value(str) {}

  std::string string_value;
};

TEST(scoped_singleton, simple_retain_no_typedef) {
  {
    auto instance = fst::scoped_singleton<MySingleton>::retain("Pear");
    EXPECT_EQ(fst::scoped_singleton<MySingleton>::get()->string_value, "Pear");
    EXPECT_EQ(fst::scoped_singleton<MySingleton>::is_retained(), true);
    EXPECT_EQ(fst::scoped_singleton<MySingleton>::get_count(), 1);
  }

  EXPECT_EQ(fst::scoped_singleton<MySingleton>::is_retained(), false);
  EXPECT_EQ(fst::scoped_singleton<MySingleton>::get_count(), 0);
}

TEST(scoped_singleton, simple_retain_moved) {
  auto instance = fst::scoped_singleton<MySingleton>::retain("Pear");
  {
    auto instance2 = fst::scoped_singleton<MySingleton>::retain();
    EXPECT_EQ(fst::scoped_singleton<MySingleton>::get()->string_value, "Pear");
    EXPECT_EQ(fst::scoped_singleton<MySingleton>::is_retained(), true);
    EXPECT_EQ(fst::scoped_singleton<MySingleton>::get_count(), 2);

    instance = std::move(instance2);
    EXPECT_EQ(fst::scoped_singleton<MySingleton>::is_retained(), true);
    EXPECT_EQ(fst::scoped_singleton<MySingleton>::get_count(), 1);
    EXPECT_EQ((bool)instance2, false);
  }

  EXPECT_EQ(fst::scoped_singleton<MySingleton>::is_retained(), true);
  EXPECT_EQ(fst::scoped_singleton<MySingleton>::get_count(), 1);
}

using MyScopedSingleton = fst::scoped_singleton<MySingleton>;
using MySingletonInstance = MyScopedSingleton::shared;

struct MySingletonInitializer {
  inline static void init(const std::string& str) { MyScopedSingleton::get()->string_value = str; }
  inline static void init() { MyScopedSingleton::get()->string_value = "Peter"; }
};

using MySingletonRetainer = fst::scoped_singleton_retainer<MySingleton, MySingletonInitializer>;

TEST(scoped_singleton, simple_retain) {
  EXPECT_EQ(MyScopedSingleton::is_retained(), false);
  EXPECT_EQ(MyScopedSingleton::get_count(), 0);

  // Create an instance of the singleton.
  {
    MySingletonInstance instance = MyScopedSingleton::retain();
    instance->string_value = "Banana";
    EXPECT_EQ(MyScopedSingleton::get()->string_value, "Banana");
    EXPECT_EQ(MyScopedSingleton::is_retained(), true);
    EXPECT_EQ(MyScopedSingleton::get_count(), 1);
  }

  // The only instance got destroyed.

  EXPECT_EQ(MyScopedSingleton::is_retained(), false);
  EXPECT_EQ(MyScopedSingleton::get_count(), 0);

  if constexpr (fst::config::has_assert) {
    EXPECT_DEATH({ MyScopedSingleton::get(); }, "");
  }
  else {
    EXPECT_EQ(MyScopedSingleton::get(), nullptr);
  }

  // Recreate an instance of the singleton, make sure it gets constructed properly.
  MySingletonInstance instance = MyScopedSingleton::retain("Potato");
  EXPECT_EQ(MyScopedSingleton::get()->string_value, "Potato");
  EXPECT_EQ(MyScopedSingleton::is_retained(), true);
  EXPECT_EQ(MyScopedSingleton::get_count(), 1);
}

TEST(scoped_singleton, simple_retain_with_args) {
  MySingletonInstance instance = MyScopedSingleton::retain("Banana");
  EXPECT_EQ(MyScopedSingleton::get()->string_value, "Banana");
}

TEST(scoped_singleton, simple_retain_func) {
  MySingletonInstance instance
      = MyScopedSingleton::retain_with_initializer([]() { MyScopedSingleton::get()->string_value = "Banana"; });
  EXPECT_EQ(MyScopedSingleton::get()->string_value, "Banana");
}

TEST(scoped_singleton, simple_retain_func_with_pointer) {
  // "Peter" is passed as parameter to MySingleton constructor, then the init function is called.
  MySingletonInstance instance = MyScopedSingleton::retain_with_initializer(
      [](MySingleton* my_singleton) {
        EXPECT_EQ(my_singleton->string_value, "Peter");
        my_singleton->string_value = "Banana";
      },
      "Peter");
  EXPECT_EQ(MyScopedSingleton::get()->string_value, "Banana");

  // This initializer won't be called since the singleton was already retained and init.
  // "Apple" would be passed as parameter to MySingleton constructor if it wasn't already retained.
  MyScopedSingleton::retain_with_initializer(
      [](MySingleton* my_singleton) { my_singleton->string_value = "Jason"; }, "Apple");

  EXPECT_EQ(MyScopedSingleton::get()->string_value, "Banana");
}

TEST(scoped_singleton, double_retain) {
  MySingletonInstance instance2;

  {
    MySingletonInstance instance = MyScopedSingleton::retain();
    instance->string_value = "Banana";

    EXPECT_EQ(MyScopedSingleton::get()->string_value, "Banana");
    EXPECT_EQ(MyScopedSingleton::is_retained(), true);
    EXPECT_EQ(MyScopedSingleton::get_count(), 1);

    // Retain singleton in instance2.
    // "Peach" would be passed as parameter to MySingleton constructor if it wasn't already retained.
    // "Banana" should remain as string_value.
    instance2 = MyScopedSingleton::retain("Peach");

    EXPECT_EQ(MyScopedSingleton::is_retained(), true);
    EXPECT_EQ(MyScopedSingleton::get_count(), 2);
    EXPECT_EQ(MyScopedSingleton::get()->string_value, "Banana");
  }

  EXPECT_EQ(MyScopedSingleton::is_retained(), true);
  EXPECT_EQ(MyScopedSingleton::get_count(), 1);
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
