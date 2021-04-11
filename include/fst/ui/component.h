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
#include "fst/ui/context.h"
#include "fst/ui/mouse_event.h"
#include "fst/print.h"
#include "fst/util.h"

#include <limits>
#include <memory>
#include <vector>

#include <bitset>
namespace fst {
template <class Enum, size_t N = size_t(Enum::count)>
class enum_bitset {
public:
  using enum_type = Enum;

  inline bool operator[](enum_type e) const { return _bitset.test(static_cast<std::size_t>(e)); }

  inline void set(enum_type e, bool value) { _bitset.set(static_cast<std::size_t>(e), value); }

  inline void reset() { _bitset.reset(); }

  inline void set_all() {
    _bitset.reset();
    _bitset.flip();
  }

private:
  std::bitset<N> _bitset;
};
} // namespace fst.

namespace fst::ui {
class component;

namespace detail {
  class component_manager {
  public:

    inline void release(fst::ui::component* c);
    
    inline void set_selected_component(fst::ui::component* c, const fst::ui::mouse_event& evt);

  private:
    fst::ui::component* _selected_component = nullptr;
  };
  
  class component_internal_ops {
  public:
    inline component_internal_ops(component* c);
    
    inline void set_manager(component_manager* manager);
    
    struct found_component_info {
      fst::ui::component* component;
      fst::ui::fpoint relative_position;
      
      inline explicit operator bool() const {
        return component != nullptr;
      }
    };

    
    inline found_component_info find_under_position(const fst::ui::fpoint& p);

    inline void handle_mouse_down(const fst::ui::mouse_event& evt);
    inline void handle_mouse_up(const fst::ui::mouse_event& evt);
    inline void handle_mouse_move(const fst::ui::mouse_event& evt);
    inline void handle_mouse_drag(const fst::ui::mouse_event& evt);
    inline void handle_mouse_wheel(const fst::ui::mouse_event& evt);
    
    inline void handle_mouse_enter(const fst::ui::mouse_event& evt);
    inline void handle_mouse_leave();

    inline void handle_paint(fst::ui::context& g);

  private:
    component* _component;
  };
  
  
  
} // namespace detail.

class component {
public:
  enum class id_type : std::size_t {};
  static constexpr id_type invalid_id = std::numeric_limits<id_type>::max();
  static constexpr std::size_t invalid_index = std::numeric_limits<std::size_t>::max();

  enum class bounds_status { bounds, position, size };

  // clang-format off
  enum class override_info {
    parent_changed,
    children_changed,
    bounds_changed,

    mouse_down,
    mouse_up,
    mouse_move,
    mouse_drag,
    mouse_wheel,
    mouse_enter,
    mouse_leave,

    paint,
    count
  };
  // clang-format on


  inline component() {
    _id = generate_id();
    _override_info.set_all();
  }

  inline ~component() {
    if (component* p = get_parent()) {
      p->remove(this);
    }
    
    if(_manager) {
      _manager->release(this);
    }
  }

  inline void add(component* c) {
    // Remove from old parent.
    if (component* p = c->get_parent()) {
      p->remove(c, false);
    }
    
    // Release from old manager.
    if(c->_manager && c->_manager != _manager) {
      c->_manager->release(c);
    }
    c->_manager = _manager;

    _children.push_back(c);
    _children.back()->_parent = this;

    if (c->_override_info[override_info::parent_changed]) {
      c->parent_changed();
    }

    if (_override_info[override_info::children_changed]) {
      children_changed();
    }
  }

  inline void add(component& c) { add(&c); }

  inline void add(component& c, const fst::ui::rect& bounds) {
    add(&c);
    c.set_bounds(bounds);
  }

  inline void remove(const component* c, bool notify_parent_changed = true) {
    std::size_t index = get_child_index(c);
    if (index == invalid_index) {
      return;
    }

    component* cc = _children[index];
    cc->_parent = nullptr;
    _children.erase(_children.begin() + index);
    if(cc->_manager) {
      cc->_manager->release(this);
      cc->_manager = nullptr;
    }

    if (_override_info[override_info::parent_changed] && notify_parent_changed) {
      cc->parent_changed();
    }

    if (_override_info[override_info::children_changed]) {
      children_changed();
    }
  }

  inline bool has_child(const component* c) const { return get_child_index(c) != invalid_index; }

  inline bool has_child(id_type c_id) const { return get_child_index(c_id) != invalid_index; }

  inline std::size_t get_child_index(const component* c) const {
    for (std::size_t i = 0; i < _children.size(); i++) {
      if (_children[i] == c) {
        return i;
      }
    }

    return invalid_index;
  }

  inline std::size_t get_child_index(id_type c_id) const {
    for (std::size_t i = 0; i < _children.size(); i++) {
      if (_children[i]->get_id() == c_id) {
        return i;
      }
    }

    return invalid_index;
  }

  inline id_type get_id() const { return _id; }

  inline component* get_parent() { return _parent; }

  inline const component* get_parent() const { return _parent; }

  inline component* get_root_parent() {
    component* p = _parent;

    while (p->get_parent()) {
      p = p->get_parent();
    }

    return p;
  }

  inline const component* get_root_parent() const {
    const component* p = _parent;

    while (p->get_parent()) {
      p = p->get_parent();
    }

    return p;
  }

  inline void set_position(const fst::ui::point& p) {
    if (fst::assign(_bounds.position, p) && _override_info[override_info::bounds_changed]) {
      bounds_changed(bounds_status::position);
    }
  }

  inline void set_size(const fst::ui::size& s) {
    if (fst::assign(_bounds.size, s) && _override_info[override_info::bounds_changed]) {
      bounds_changed(bounds_status::size);
    }
  }

  inline void set_bounds(const fst::ui::rect& r) {
    if (!_override_info[override_info::bounds_changed]) {
      _bounds = r;
      return;
    }

    bool p_changed = fst::assign(_bounds.position, r.position);
    bool s_changed = fst::assign(_bounds.size, r.size);

    if (p_changed && s_changed) {
      bounds_changed(bounds_status::bounds);
    }
    else if (p_changed) {
      bounds_changed(bounds_status::position);
    }
    else if (s_changed) {
      bounds_changed(bounds_status::size);
    }
  }

  inline const fst::ui::point& get_position() const { return _bounds.position; }

  inline const fst::ui::size& get_size() const { return _bounds.size; }

  inline const fst::ui::rect& get_bounds() const { return _bounds; }

  inline fst::ui::rect get_local_bounds() const { return fst::ui::rect({ 0, 0 }, _bounds.size); }

  inline void set_clipping(bool clipping) { _is_clipped = clipping; }

  inline bool is_clipped() const { return _is_clipped; }

  virtual void parent_changed() {}
  virtual void children_changed() {}
  virtual void bounds_changed(bounds_status bs) {}

  // Mouse events.
  virtual void mouse_down(const fst::ui::mouse_event& evt) {}
  virtual void mouse_up(const fst::ui::mouse_event& evt) {}
  virtual void mouse_move(const fst::ui::mouse_event& evt) {}
  virtual void mouse_drag(const fst::ui::mouse_event& evt) {}
  virtual void mouse_wheel(const fst::ui::mouse_event& evt) {}

  /// When the mouse button is pressed and held down while being moved in or out of a component,
  /// none of mouse_enter or mouse_leave are called.
  virtual void mouse_enter(const fst::ui::mouse_event& evt) {}
  virtual void mouse_leave() {}

  virtual void paint(fst::ui::context& gc) {}

public:
  template <typename T>
  inline void init() {
#define HAS_OVERRIDE(fct) !std::is_same_v<decltype(&component::fct), decltype(&T::fct)>
    constexpr bool has_parent_changed = HAS_OVERRIDE(parent_changed);
    constexpr bool has_children_changed = HAS_OVERRIDE(children_changed);
    constexpr bool has_bounds_changed = HAS_OVERRIDE(bounds_changed);
    constexpr bool has_mouse_down = HAS_OVERRIDE(mouse_down);
    constexpr bool has_mouse_up = HAS_OVERRIDE(mouse_up);
    constexpr bool has_mouse_move = HAS_OVERRIDE(mouse_move);
    constexpr bool has_mouse_drag = HAS_OVERRIDE(mouse_drag);
    constexpr bool has_mouse_wheel = HAS_OVERRIDE(mouse_wheel);
    constexpr bool has_mouse_enter = HAS_OVERRIDE(mouse_enter);
    constexpr bool has_mouse_leave = HAS_OVERRIDE(mouse_leave);
    constexpr bool has_paint = HAS_OVERRIDE(paint);
#undef HAS_OVERRIDE

    _override_info.set(override_info::parent_changed, has_parent_changed);
    _override_info.set(override_info::children_changed, has_children_changed);
    _override_info.set(override_info::bounds_changed, has_bounds_changed);

    // Mouse.
    _override_info.set(override_info::mouse_down, has_mouse_down);
    _override_info.set(override_info::mouse_up, has_mouse_up);
    _override_info.set(override_info::mouse_move, has_mouse_move);
    _override_info.set(override_info::mouse_drag, has_mouse_drag);
    _override_info.set(override_info::mouse_wheel, has_mouse_wheel);
    _override_info.set(override_info::mouse_enter, has_mouse_enter);
    _override_info.set(override_info::mouse_leave, has_mouse_leave);

    // Paint.
    _override_info.set(override_info::paint, has_paint);
  }
  
  inline bool get_override_info(override_info o_info) const {
    return _override_info[o_info];
  }

  inline void repaint() {
    _needs_repaint = true;
    _bounds = get_local_bounds();
  }

  inline void repaint(const fst::ui::rect& r) {
    _needs_repaint = true;
    _bounds = r;
    _bounds.clip_to(get_local_bounds());
  }

private:
  friend class detail::component_internal_ops;

  std::vector<component*> _children;
  fst::enum_bitset<override_info> _override_info;
  fst::ui::rect _dirty_bounds;
  component* _parent = nullptr;
  detail::component_manager* _manager = nullptr;
  fst::ui::rect _bounds;
  id_type _id;
  bool _is_clipped = true;
  bool _needs_repaint = true;

  inline static id_type generate_id() {
    static id_type __id = (id_type)0;
    __id = id_type(std::size_t(__id) + 1);
    return __id;
  }
};

namespace detail {
  inline void component_manager::release(fst::ui::component* c) {
    if(_selected_component == c) {
      _selected_component = nullptr;
    }
  }

  inline void component_manager::set_selected_component(fst::ui::component* c, const fst::ui::mouse_event& evt) {
    if(_selected_component == c) {
      return;
    }

    if(_selected_component) {
      component_internal_ops(_selected_component).handle_mouse_leave();
      _selected_component = nullptr;
    }
    
    if(c == nullptr) {
      return;
    }
    
    _selected_component = c;
    component_internal_ops(_selected_component).handle_mouse_enter(evt);
  }
  
  //---------------------------------------------------------
  inline component_internal_ops::component_internal_ops(component* c)
      : _component(c) {}

  inline void component_internal_ops::set_manager(component_manager* manager) {
    _component->_manager = manager;
  }

  inline component_internal_ops::found_component_info component_internal_ops::find_under_position(const fst::ui::fpoint& p) {
    if(!_component->get_bounds().contains(p)) {
      return {nullptr, fst::ui::fpoint(0, 0)};
    }
    
    fst::ui::fpoint c_pos = p - _component->get_position();
    
    // Look for child.
    for (auto& c : _component->_children) {
      if(found_component_info c_info = component_internal_ops(c).find_under_position(c_pos)) {
        return c_info;
      }
    }

    return {_component, c_pos};
  }

  inline void component_internal_ops::handle_mouse_down(const fst::ui::mouse_event& evt) {
    if(_component->get_override_info(fst::ui::component::override_info::mouse_down)) {
      _component->mouse_down(evt);
    }
  }
  
  inline void component_internal_ops::handle_mouse_up(const fst::ui::mouse_event& evt) {
    if(_component->get_override_info(fst::ui::component::override_info::mouse_up)) {
      _component->mouse_up(evt);
    }
  }
  
  inline void component_internal_ops::handle_mouse_move(const fst::ui::mouse_event& evt) {
    if(_component->get_override_info(fst::ui::component::override_info::mouse_move)) {
      _component->mouse_move(evt);
    }
  }
  
  inline void component_internal_ops::handle_mouse_drag(const fst::ui::mouse_event& evt) {
    if(_component->get_override_info(fst::ui::component::override_info::mouse_drag)) {
      _component->mouse_drag(evt);
    }
  }
  
  inline void component_internal_ops::handle_mouse_wheel(const fst::ui::mouse_event& evt) {
  }
  
  inline void component_internal_ops::handle_mouse_enter(const fst::ui::mouse_event& evt) {
    if(_component->get_override_info(fst::ui::component::override_info::mouse_enter)) {
      _component->mouse_enter(evt);
    }
  }
  
  inline void component_internal_ops::handle_mouse_leave() {
    if(_component->get_override_info(fst::ui::component::override_info::mouse_leave)) {
      _component->mouse_leave();
    }
  }
  

  inline void component_internal_ops::handle_paint(fst::ui::context& g) {
    g.save_state();

    g.translate(_component->get_position());

    if (_component->is_clipped()) {
      g.clip_to_rect(_component->get_local_bounds());
    }

    if (_component->_needs_repaint) {
      _component->_needs_repaint = false;
      _component->paint(g);
    }

    for (auto& c : _component->_children) {
      component_internal_ops(c).handle_paint(g);
    }

    g.restore_state();
  }
} // namespace detail.

namespace detail {

}
} // namespace fst::ui.
