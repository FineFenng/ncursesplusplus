#ifndef NCURSESPLUSPLUS_SYSTEMEvent_NAME_HPP
#define NCURSESPLUSPLUS_SYSTEMEvent_NAME_HPP
#include <string>

#include "ncursesplusplus/system/event.hpp"

Namespace ox {

/// Return the Name of the given Event type.
inline auto Name(PaintEvent const &) -> std::string { return "PaintEvent"; }

inline auto Name(KeyPressEvent const &) -> std::string {
  return "KeyPressEvent";
}

inline auto Name(MousePressEvent const &) -> std::string {
  return "MousePressEvent";
}

inline auto Name(MouseReleaseEvent const &) -> std::string {
  return "MouseReleaseEvent";
}

inline auto Name(MouseDoubleClickEvent const &) -> std::string {
  return "MouseDoubleClickEvent";
}

inline auto Name(MouseWheelEvent const &) -> std::string {
  return "MouseWheelEvent";
}

inline auto Name(MouseMoveEvent const &) -> std::string {
  return "MouseMoveEvent";
}

inline auto Name(ChildAddedEvent const &) -> std::string {
  return "ChildAddedEvent";
}

inline auto Name(ChildRemovedEvent const &) -> std::string {
  return "ChildRemovedEvent";
}

inline auto Name(ChildPolishedEvent const &) -> std::string {
  return "ChildPolishedEvent";
}

inline auto Name(DeleteEvent const &) -> std::string { return "DeleteEvent"; }

inline auto Name(DisableEvent const &) -> std::string {
  return "DisableEvent";
}

inline auto Name(EnableEvent const &) -> std::string { return "EnableEvent"; }

inline auto Name(FocusInEvent const &) -> std::string {
  return "FocusInEvent";
}

inline auto Name(FocusOutEvent const &) -> std::string {
  return "FocusOutEvent";
}

inline auto Name(MoveEvent const &) -> std::string { return "MoveEvent"; }

inline auto Name(ResizeEvent const &) -> std::string { return "ResizeEvent"; }

inline auto Name(TimerEvent const &) -> std::string { return "TimerEvent"; }

inline auto Name(CustomEvent const &) -> std::string { return "CustomEvent"; }

}  // Namespace ox
#endif  // NCURSESPLUSPLUS_SYSTEMEvent_NAME_HPP
