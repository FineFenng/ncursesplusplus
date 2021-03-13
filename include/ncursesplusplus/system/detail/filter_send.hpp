#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_FILTER_SEND_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_FILTER_SEND_HPP
#include <algorithm>
#include <iterator>

#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/widget/widget.hpp"

namespace npp {
namespace detail {

/// Applies \p filter_function over \p filters, up until it returns true.
/** If none return true, then this returns false. */
template<typename F>
bool apply_until_accepted(F &&filter_function, std::set<Widget *> const &filters) {
  return std::find_if(std::begin(filters), std::end(filters),
                      filter_function) != std::end(filters);
}

inline bool filter_send(npp::PaintEvent const &e) {
  return apply_until_accepted(
      [&e](Widget *filter) { return filter->paint_event_filter(e.receiver); },
      e.receiver.get().get_event_filters());
}

inline bool filter_send(npp::KeyPressEvent const &e) {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->key_press_event_filter(e.receiver, e.key);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::MousePressEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->mouse_press_event_filter(e.receiver, e.data);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::MouseReleaseEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->mouse_release_event_filter(e.receiver, e.data);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::MouseDoubleClickEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->mouse_double_click_event_filter(e.receiver, e.data);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::MouseWheelEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->mouse_wheel_event_filter(e.receiver, e.data);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::MouseMoveEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->mouse_move_event_filter(e.receiver, e.data);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::ChildAddedEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->child_added_event_filter(e.receiver, e.child);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::ChildRemovedEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->child_removed_event_filter(e.receiver, e.child);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::ChildPolishedEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->child_polished_event_filter(e.receiver, e.child);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::DeleteEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->delete_event_filter(*e.removed);
      },
      e.removed->get_event_filters());
}

inline auto filter_send(npp::DisableEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->disable_event_filter(e.receiver);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::EnableEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->enable_event_filter(e.receiver);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::FocusInEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->focus_in_event_filter(e.receiver);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::FocusOutEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) {
        return filter->focus_out_event_filter(e.receiver);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::MoveEvent const &e) -> bool {
  auto const old_position = e.receiver.get().top_left();
  auto const new_position = e.new_position;
  if (old_position == new_position)
    return true;
  return apply_until_accepted(
      [&](Widget *filter) {
        return filter->move_event_filter(e.receiver, new_position,
                                         old_position);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::ResizeEvent const &e) -> bool {
  auto const old_area = e.receiver.get().outer_area();
  auto const new_area = e.new_area;
  if (old_area == new_area)
    return true;

  return apply_until_accepted(
      [&](Widget *filter) {
        return filter->resize_event_filter(e.receiver, new_area, old_area);
      },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::TimerEvent const &e) -> bool {
  return apply_until_accepted(
      [&e](Widget *filter) { return filter->timer_event_filter(e.receiver); },
      e.receiver.get().get_event_filters());
}

inline auto filter_send(npp::CustomEvent const &e) -> bool {
  return e.filter_send();
}

}
}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_FILTER_SEND_HPP
