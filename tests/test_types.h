#pragma once

#include <type_traits>
#include "fst/traits.h"

namespace test {
//
// fully_trivial_type.
//
class fully_trivial_type {
public:
  fully_trivial_type() = default;
  fully_trivial_type(const fully_trivial_type&) = default;
  fully_trivial_type(fully_trivial_type&&) = default;
  ~fully_trivial_type() = default;
  fully_trivial_type& operator=(const fully_trivial_type&) = default;
  fully_trivial_type& operator=(fully_trivial_type&&) = default;

private:
  int _data;
};

static_assert(
    std::is_trivially_default_constructible<fully_trivial_type>::value, "Should be trivially default constructible");
static_assert(
    std::is_nothrow_default_constructible<fully_trivial_type>::value, "Should be nothrow default constructible");
static_assert(
    std::is_trivially_copy_constructible<fully_trivial_type>::value, "Should be trivially copy constructible");
static_assert(
    std::is_trivially_move_constructible<fully_trivial_type>::value, "Should be trivially move constructible");
static_assert(std::is_trivially_copy_assignable<fully_trivial_type>::value, "Should be trivially copy assignable");
static_assert(std::is_trivially_move_assignable<fully_trivial_type>::value, "Should be trivially move assignable");
static_assert(std::is_trivially_destructible<fully_trivial_type>::value, "Should be trivially destructible");
static_assert(std::is_nothrow_destructible<fully_trivial_type>::value, "Should be nothrow destructible");
static_assert(
    fst::traits::is_fast_vector_resize<fully_trivial_type>::value, "Should be trivially default constructible");

//
// trivial_type_no_move.
//
class trivial_type_no_move {
public:
  trivial_type_no_move() = default;
  trivial_type_no_move(const trivial_type_no_move&) = default;
  trivial_type_no_move(trivial_type_no_move&&) = delete;
  ~trivial_type_no_move() = default;
  trivial_type_no_move& operator=(const trivial_type_no_move&) = default;
  trivial_type_no_move& operator=(trivial_type_no_move&&) = delete;

private:
  int _data;
};

static_assert(
    std::is_trivially_default_constructible<trivial_type_no_move>::value, "Should be trivially default constructible");
static_assert(
    std::is_nothrow_default_constructible<trivial_type_no_move>::value, "Should be nothrow default constructible");
static_assert(
    std::is_trivially_copy_constructible<trivial_type_no_move>::value, "Should be trivially copy constructible");
static_assert(
    !std::is_trivially_move_constructible<trivial_type_no_move>::value, "Should not be trivially move constructible");
static_assert(std::is_trivially_copy_assignable<trivial_type_no_move>::value, "Should be trivially copy assignable");
static_assert(
    !std::is_trivially_move_assignable<trivial_type_no_move>::value, "Should not be trivially move assignable");
static_assert(std::is_trivially_destructible<trivial_type_no_move>::value, "Should be trivially destructible");
static_assert(std::is_nothrow_destructible<trivial_type_no_move>::value, "Should be nothrow destructible");
static_assert(
    fst::traits::is_fast_vector_resize<trivial_type_no_move>::value, "Should be trivially default constructible");
} // namespace test.
