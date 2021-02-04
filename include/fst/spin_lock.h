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
#include <atomic>

// https://livebook.manning.com/book/c-plus-plus-concurrency-in-action/chapter-7/7

namespace fst {
class spin_lock_mutex {
public:
  spin_lock_mutex() noexcept = default;
  ~spin_lock_mutex() noexcept = default;

  spin_lock_mutex(const spin_lock_mutex&) = delete;
  spin_lock_mutex(spin_lock_mutex&&) = delete;

  spin_lock_mutex& operator=(const spin_lock_mutex&) = delete;
  spin_lock_mutex& operator=(spin_lock_mutex&&) = delete;

  inline void lock() noexcept {
    while (_flag.test_and_set(std::memory_order_acquire))
      ;
  }

  inline void unlock() noexcept { _flag.clear(std::memory_order_release); }

private:
  std::atomic_flag _flag = ATOMIC_FLAG_INIT;
};

class scoped_spin_lock {
public:
  inline scoped_spin_lock(spin_lock_mutex& mutex)
      : _mutex(mutex) {
    _mutex.lock();
  }

  inline ~scoped_spin_lock() { _mutex.unlock(); }

  scoped_spin_lock(const scoped_spin_lock&) = delete;
  scoped_spin_lock(scoped_spin_lock&&) = delete;

  scoped_spin_lock& operator=(const scoped_spin_lock&) = delete;
  scoped_spin_lock& operator=(scoped_spin_lock&&) = delete;

private:
  spin_lock_mutex& _mutex;
};
} // namespace fst.
