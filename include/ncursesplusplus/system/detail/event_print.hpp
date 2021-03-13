#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_EVENT_PRINT_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_EVENT_PRINT_HPP
#include <iostream>

#include "ncursesplusplus/system/event.hpp"

namespace ox::detail {

inline void event_print(std::ostream &os, ox::Paint_event const &e) {
  os << "PaintEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Key_press_event const &e) {
  os << "KeyPressEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Mouse_press_event const &e) {
  os << "MousePressEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Mouse_release_event const &e) {
  os << "MouseReleaseEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Mouse_double_click_event const &e) {
  os << "MouseDoubleClickEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Mouse_wheel_event const &e) {
  os << "MouseWheelEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Mouse_move_event const &e) {
  os << "MouseMoveEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Child_added_event const &e) {
  os << "ChildAddedEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Child_removed_event const &e) {
  os << "ChildRemovedEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Child_polished_event const &e) {
  os << "ChildPolishedEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Delete_event const &e) {
  os << "DeleteEvent\n";
  os << "--->receiver id:   " << e.removed->unique_id() << '\n';
  os << "--->receiver name: " << e.removed->name() << '\n';
}

inline void event_print(std::ostream &os, ox::Disable_event const &e) {
  os << "DisableEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Enable_event const &e) {
  os << "EnableEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Focus_in_event const &e) {
  os << "FocusInEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Focus_out_event const &e) {
  os << "FocusOutEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Move_event const &e) {
  os << "MoveEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Resize_event const &e) {
  os << "ResizeEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Timer_event const &e) {
  os << "TimerEvent\n";
  os << "--->receiver id:   " << e.receiver.get().unique_id() << '\n';
  os << "--->receiver name: " << e.receiver.get().name() << '\n';
}

inline void event_print(std::ostream &os, ox::Custom_event const &) {
  os << "CustomEvent\n";
}

}  // namespace ox::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_EVENT_PRINT_HPP
