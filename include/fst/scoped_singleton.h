#pragma once

#include <memory>
#include <type_traits>

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
    return get_weak().lock().get();
  }

  template <typename... Args>
  inline static shared_ptr_type retain(Args&&... args) {
    return get_instance(std::forward<Args>(args)...);
  }

  template <typename _Fct, typename... Args>
  inline static shared_ptr_type retain_with_initializer(_Fct fct, Args&&... args) {
    bool needs_init = !is_retained();
    shared_ptr_type instance = get_instance(std::forward<Args>(args)...);

    if (needs_init) {
      if constexpr (std::is_invocable<_Fct, pointer>::value) {
        fct(instance.get());
      }
      else if constexpr (std::is_invocable<_Fct>::value) {
        fct();
      }
    }

    return instance;
  }

  inline static bool is_retained() { return !get_weak().expired(); }
  inline static std::size_t get_count() { return get_weak().use_count(); }

private:
  scoped_singleton() = default;
  scoped_singleton(const scoped_singleton&) = delete;
  scoped_singleton(scoped_singleton&&) = delete;
  scoped_singleton& operator=(const scoped_singleton&) = delete;
  scoped_singleton& operator=(scoped_singleton&&) = delete;

  inline static weak_ptr_type& get_weak() {
    static weak_ptr_type __instance_ref;
    return __instance_ref;
  }

  template <typename... Args>
  inline static shared_ptr_type get_instance(Args&&... args) {
    weak_ptr_type& __instance_ref = get_weak();
    if (!__instance_ref.use_count()) {
      shared_ptr_type instance = std::make_shared<value_type>(std::forward<Args>(args)...);
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

  inline pointer operator->() {
    fst_assert((bool)_instance_ptr, "Can't access scoped_singleton_retainer if it is not retained somewhere.");
    return _instance_ptr.get();
  }

  inline const_pointer operator->() const {
    fst_assert((bool)_instance_ptr, "Can't access scoped_singleton_retainer if it is not retained somewhere.");
    return _instance_ptr.get();
  }

  inline void release() { _instance_ptr.reset(); }

private:
  shared_ptr_type _instance_ptr;
};
} // namespace fst.
