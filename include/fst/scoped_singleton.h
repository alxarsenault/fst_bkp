#pragma once

#include <memory>
#include "fst/assert.h"

namespace fst {
template <typename _Tp>
class scoped_singleton {
public:
  using value_type = _Tp;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using weak_ptr_type = std::weak_ptr<value_type>;
  using shared_ptr_type = std::shared_ptr<value_type>;

  ~scoped_singleton() = default;

  inline static pointer get() {
    fst_assert(is_retained(), "Can't access scoped_singleton if it is not retained somewhere.");
    return get_weak_instance().lock().get();
  }

  inline static shared_ptr_type retain() { return internal_get_instance(); }

  inline static bool is_retained() { return get_weak_instance().use_count(); }

private:
  scoped_singleton() = default;
  scoped_singleton(const scoped_singleton&) = delete;
  scoped_singleton(scoped_singleton&&) = delete;
  scoped_singleton& operator=(const scoped_singleton&) = delete;
  scoped_singleton& operator=(scoped_singleton&&) = delete;

  inline static weak_ptr_type& get_weak_instance() {
    static weak_ptr_type __instance_ref;
    return __instance_ref;
  }

  inline static shared_ptr_type internal_get_instance() {
    weak_ptr_type& __instance_ref = get_weak_instance();
    if (!__instance_ref.use_count()) {
      shared_ptr_type instance(new value_type);
      __instance_ref = instance;
      return instance;
    }

    return __instance_ref.lock();
  }
};

template <typename _Tp, typename _Initializer = void>
class scoped_singleton_retainer {
public:
  using value_type = _Tp;
  using initializer = _Initializer;

  using singleton_type = scoped_singleton<value_type>;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using shared_ptr_type = typename singleton_type::shared_ptr_type;

  template <typename... Args>
  inline scoped_singleton_retainer(Args&&... args) {
    if constexpr (std::is_same<initializer, void>::value) {
      _instance_ptr = singleton_type::retain();
    }
    else {
      bool needs_init = !singleton_type::is_retained();
      _instance_ptr = singleton_type::retain();

      if (needs_init) {
        initializer::init(std::forward<Args>(args)...);
      }
    }
  }

  scoped_singleton_retainer(const scoped_singleton_retainer&) = default;
  scoped_singleton_retainer(scoped_singleton_retainer&&) = default;

  scoped_singleton_retainer& operator=(const scoped_singleton_retainer&) = default;
  scoped_singleton_retainer& operator=(scoped_singleton_retainer&&) = default;

  pointer operator->() {
    fst_assert((bool)_instance_ptr, "Can't access scoped_singleton_retainer if it is not retained somewhere.");
    return _instance_ptr.get();
  }

  const_pointer operator->() const {
    fst_assert((bool)_instance_ptr, "Can't access scoped_singleton_retainer if it is not retained somewhere.");
    return _instance_ptr.get();
  }

  inline void release() { _instance_ptr.reset(); }

private:
  shared_ptr_type _instance_ptr;
};
} // namespace fst.
