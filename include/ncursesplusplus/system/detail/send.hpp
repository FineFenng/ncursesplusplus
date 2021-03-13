#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_SEND_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_SEND_HPP
#include "ncursesplusplus/painter/detail/is_paintable.hpp"
#include "ncursesplusplus/system/detail/focus.hpp"
#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/system/key.hpp"
#include "ncursesplusplus/widget/area.hpp"
#include "ncursesplusplus/widget/widget.hpp"

namespace npp {
namespace detail {

inline void Send(PaintEvent event) {
  if (is_paintable(event.receiver)) {
    event.receiver.get().paint_event();
  }
}

inline void Send(KeyPressEvent event) {
  event.receiver.get().key_press_event(event.key);
  switch (event.key) {
    case Key::Tab: detail::Focus::TabPress();
      break;
    case Key::Back_tab: detail::Focus::ShiftTabPress();
      break;
    default: break;
  }
}

inline void Send(MousePressEvent e) {
  detail::Focus::MousePress(e.receiver);
  e.receiver.get().mouse_press_event(e.data);
}

inline void Send(MouseReleaseEvent e) {
  e.receiver.get().mouse_release_event(e.data);
}

inline void Send(MouseDoubleClickEvent e) {
  e.receiver.get().mouse_double_click_event(e.data);
}

inline void Send(MouseWheelEvent e) {
  e.receiver.get().mouse_wheel_event(e.data);
}

inline void Send(MouseMoveEvent e) {
  e.receiver.get().mouse_move_event(e.data);
}

inline void Send(ChildAddedEvent e) {
  e.receiver.get().child_added_event(e.child);
}

inline void Send(ChildRemovedEvent e) {
  e.receiver.get().child_removed_event(e.child);
}

inline void Send(ChildPolishedEvent e) {
  e.receiver.get().child_polished_event(e.child);
}

inline void Send(DeleteEvent e) {
  if (e.removed == nullptr)
    return;
  e.removed->delete_event();
  for (Widget *w : e.removed->get_descendants())
    w->delete_event();
  e.removed.reset();
}

inline void Send(DisableEvent e) {
  e.receiver.get().screen_state().clear();
  e.receiver.get().disable_event();
}

inline void Send(EnableEvent e) { e.receiver.get().enable_event(); }

inline void Send(FocusInEvent e) {
  Focus::DirectSetFocus(&e.receiver.get());
  e.receiver.get().focus_in_event();
}

inline void Send(FocusOutEvent e) { e.receiver.get().focus_out_event(); }

inline void Send(MoveEvent e) {
  auto const old_position = e.receiver.get().top_left();
  auto const new_position = e.new_position;
  if (old_position == new_position)
    return;
  e.receiver.get().screen_state().clear();
  e.receiver.get().screen_state().move(new_position);
  e.receiver.get().set_top_left(new_position);
  e.receiver.get().move_event(new_position, old_position);
}

inline void Send(ResizeEvent e) {
  auto const old_area = e.receiver.get().outer_area();
  auto const new_area = e.new_area;
  if (old_area == new_area)
    return;
  e.receiver.get().set_outer_area(new_area);
  e.receiver.get().screen_state().resize(new_area);
  e.receiver.get().resize_event(new_area, old_area);
}

inline void Send(TimerEvent e) {
  if (detail::is_paintable(e.receiver))
    e.receiver.get().timer_event();
}

inline void Send(CustomEvent e) { e.send(); }

}
}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_SEND_HPP
