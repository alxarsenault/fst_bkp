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
