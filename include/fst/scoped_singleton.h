///
/// BSD 3-Clause License
///
/// Copyright (c) 2020, Alexandre Arsenault
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// * Redistributions of source code must retain the above copyright notice, this
///   list of conditions and the following disclaimer.
///
/// * Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
///
/// * Neither the name of the copyright holder nor the names of its
///   contributors may be used to endorse or promote products derived from
///   this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
///

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
  using weak = std::weak_ptr<value_type>;
  using shared = std::shared_ptr<value_type>;

  ~scoped_singleton() = default;

  inline static pointer get() {
    fst_assert(is_retained(), "Can't access scoped_singleton if it is not retained somewhere.");
    return get_weak().lock().get();
  }

  template <typename... Args>
  inline static shared retain(Args&&... args) {
    return get_instance(std::forward<Args>(args)...);
  }

  template <typename _Fct, typename... Args>
  inline static shared retain_with_initializer(_Fct fct, Args&&... args) {
    bool needs_init = !is_retained();
    shared instance = get_instance(std::forward<Args>(args)...);

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

  void* operator new(size_t size) = delete;
  void operator delete(void* ptr) = delete;

  inline static weak& get_weak() {
    static weak __instance_ref;
    return __instance_ref;
  }

  template <typename... Args>
  inline static shared get_instance(Args&&... args) {
    weak& __instance_ref = get_weak();
    if (!__instance_ref.use_count()) {
      static_assert(std::is_constructible<value_type, Args...>::value, "Can't construct object in scoped_singleton.");
      shared instance = std::make_shared<value_type>(std::forward<Args>(args)...);
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
  using shared = typename singleton_type::shared;

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
  shared _instance_ptr;
};
} // namespace fst.
