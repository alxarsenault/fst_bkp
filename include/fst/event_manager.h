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
#include "fst/spin_lock.h"
#include "fst/fixed_unordered_set.h"
#include "fst/fixed_vector.h"
#include "fst/pointer.h"
#include "fst/traits.h"

#include <atomic>
#include <chrono>
#include <thread>
#include <functional>

namespace fst {
class event_manager {
public:
  static constexpr std::size_t maximum_events_count = 64;
  using idle_ms_type = std::chrono::milliseconds::rep;
  static constexpr idle_ms_type default_idle_ms = 5;
  static constexpr idle_ms_type minimum_idle_ms = 1;
  static constexpr idle_ms_type maximum_idle_ms = 500;
  enum class event_id : std::size_t {};
  static constexpr event_id invalid_id = (event_id)std::numeric_limits<std::size_t>::max();

  class disconnector {
  public:
    disconnector() noexcept = default;

    disconnector(event_manager& evt_manager, event_id __id)
        : _evt_manager(&evt_manager)
        , _id(__id) {}

    disconnector(fst::not_null<event_manager*> evt_manager, event_id __id)
        : _evt_manager(evt_manager)
        , _id(__id) {}

    disconnector(const disconnector&) = delete;
    inline disconnector(disconnector&& d) noexcept
        : _evt_manager(d._evt_manager)
        , _id(d._id) {
      d._evt_manager = nullptr;
      d._id = invalid_id;
    }

    inline ~disconnector() noexcept {
      if (is_valid()) {
        _evt_manager->remove_event(_id);
      }
    }

    disconnector& operator=(const disconnector&) = delete;
    inline disconnector& operator=(disconnector&& d) noexcept {
      _evt_manager = d._evt_manager;
      _id = d._id;
      d._evt_manager = nullptr;
      d._id = invalid_id;
      return *this;
    }

    inline void disconnect() noexcept {
      if (is_valid()) {
        _evt_manager->remove_event(_id);
        _evt_manager = nullptr;
        _id = invalid_id;
      }
    }

    inline event_id get_id() const noexcept { return _id; }

    inline event_manager* get_event_manager() noexcept { return _evt_manager; }

    inline const event_manager* get_event_manager() const noexcept { return _evt_manager; }

    inline bool is_valid() const noexcept { return _evt_manager && _id != invalid_id; }

    inline bool is_connected() const noexcept { return is_valid() && _evt_manager->is_connected(_id); }

  private:
    event_manager* _evt_manager = nullptr;
    event_id _id = invalid_id;
  };

  inline event_manager() = default;
  inline ~event_manager() { stop(); }

  inline void init(idle_ms_type delta_time_ms = default_idle_ms) {
    stop();
    _delta_time_ms = std::chrono::milliseconds(std::clamp(delta_time_ms, minimum_idle_ms, maximum_idle_ms));
    _stopper.start();
    _idle_thread = std::thread(&event_manager::idle_thread, std::ref(*this));
  }

  inline idle_ms_type get_delta_time() const { return _delta_time_ms.count(); }

  inline event_id add_event(const std::function<void()>& fct) { return (event_id)_event_stack.add(fct, 0, 0, false); }

  inline event_id add_recurrent_event(const std::function<void()>& fct, std::size_t tick_count = 0) {
    return (event_id)_event_stack.add(fct, tick_count, tick_count, true);
  }

  inline event_id add_recurrent_event(
      const std::function<void()>& fct, std::size_t tick_count, std::size_t init_tick_count) {
    return (event_id)_event_stack.add(fct, tick_count, init_tick_count, true);
  }

  inline event_id add_event(
      const std::function<void()>& fct, std::size_t tick_count, std::size_t init_tick_count, bool is_recurrent) {
    return (event_id)_event_stack.add(fct, tick_count, init_tick_count, is_recurrent);
  }

  inline bool remove_event(event_id __id) { return _event_stack.remove((std::size_t)__id); }

  inline bool is_connected(event_id __id) const { return _event_stack.is_connected((std::size_t)__id); }

private:
  class event_stack {
  public:
    struct event_data {
      std::function<void()> callback;
      std::size_t id = 0;
      std::size_t count;
      std::size_t counter;
      bool is_recurrent;
    };

    //    using vector_type = std::vector<event_data>;
    using vector_type = fst::stack_fixed_vector<event_data, maximum_events_count>;

    inline std::size_t add(
        const std::function<void()>& fct, std::size_t tick_count, std::size_t init_tick_count, bool is_recurrent) {
      fst::scoped_spin_lock lock(_lock);
      _id_counter++;
      std::size_t id = _id_counter;
      _data.push_back({ fct, id, init_tick_count, tick_count, is_recurrent });
      return id;
    }

    inline bool remove(std::size_t id) {
      fst::scoped_spin_lock lock(_lock);
      for (std::size_t i = 0; i < _data.size(); i++) {
        if (_data[i].id == id) {
          _data.erase(i);
          //          _data.erase(_data.begin() + i);
          return true;
        }
      }

      return false;
    }

    inline bool is_connected(std::size_t __id) const {
      fst::scoped_spin_lock lock(_lock);
      for (const event_data& evt_data : _data) {
        if (evt_data.id == __id) {
          return true;
        }
      }

      return false;
    }

    inline void execute() {
      _lock.lock();
      for (auto& f : _data) {
        if (++f.count >= f.counter) {
          f.callback();
          f.count = 0;

          if (!f.is_recurrent) {
            _ids_to_remove.push_back(f.id);
          }
        }
      }
      _lock.unlock();

      _lock.lock();
      if (_ids_to_remove.size()) {
        std::size_t r_index = 0;
        for (auto it = _data.begin(); it != _data.end() && r_index < _ids_to_remove.size();) {
          if (it->id == _ids_to_remove[r_index]) {
            _data.erase(it);
            r_index++;
          }
          else {
            ++it;
          }
        }

        _ids_to_remove.clear();
      }
      _lock.unlock();
    }

    inline void clear() {
      fst::scoped_spin_lock lock(_lock);
      _data.clear();
    }

  private:
    vector_type _data;
    fst::unordered_array<std::size_t, maximum_events_count> _ids_to_remove;
    mutable fst::spin_lock_mutex _lock;
    std::size_t _id_counter = 0;
  };

  class thread_stopper {
  public:
    inline thread_stopper() noexcept { _stopper.test_and_set(); }
    inline ~thread_stopper() { stop(); }

    thread_stopper(const thread_stopper&) = delete;
    thread_stopper(thread_stopper&&) = delete;
    thread_stopper& operator=(const thread_stopper&) = delete;
    thread_stopper& operator=(thread_stopper&&) = delete;

    inline explicit operator bool() {
      // Atomically changes the state of a std::atomic_flag to set(true)
      // and returns the value it held before.
      return _stopper.test_and_set();
    }

    inline void start() { _stopper.test_and_set(); }
    inline void stop() { _stopper.clear(); }

  private:
    std::atomic_flag _stopper = ATOMIC_FLAG_INIT;
  };

  inline void stop() {
    _stopper.stop();

    if (_idle_thread.joinable()) {
      _idle_thread.join();
    }
  }

  static int idle_thread(event_manager& em) {
    while (em._stopper) {
      em._event_stack.execute();
      std::this_thread::sleep_for(em._delta_time_ms);
    }

    return 0;
  }

  event_stack _event_stack;
  thread_stopper _stopper;
  std::thread _idle_thread;
  std::chrono::milliseconds _delta_time_ms;
};
} // namespace fst.
