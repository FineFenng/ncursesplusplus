#ifndef NCURSESPLUSPLUS_SYSTEM_EVENT_HPP
#define NCURSESPLUSPLUS_SYSTEM_EVENT_HPP
#include <functional>
#include <memory>

#include "ncursesplusplus/system/key.hpp"
#include "ncursesplusplus/system/mouse.hpp"
#include "ncursesplusplus/widget/area.hpp"
#include "ncursesplusplus/widget/point.hpp"
#include "ncursesplusplus/widget/widget.hpp"

#include "absl/types/variant.h"


namespace npp {

using WidgetWrapper = std::reference_wrapper<Widget>;

struct PaintEvent {
  WidgetWrapper receiver;
};

struct KeyPressEvent {
  WidgetWrapper receiver;
  npp::Key key;
};

struct MousePressEvent{
  WidgetWrapper receiver;
  Mouse data;
};

struct MouseReleaseEvent {
  WidgetWrapper receiver;
  Mouse data;
};

struct MouseDoubleClickEvent {
  WidgetWrapper receiver;
  Mouse data;
};

struct MouseWheelEvent {
  WidgetWrapper receiver;
  Mouse data;
};

struct MouseMoveEvent {
  WidgetWrapper receiver;
  Mouse data;
};

struct ChildAddedEvent {
  WidgetWrapper receiver;
  WidgetWrapper child;
};

struct ChildRemovedEvent {
  WidgetWrapper receiver;
  WidgetWrapper child;
};

struct ChildPolishedEvent {
  WidgetWrapper receiver;
  WidgetWrapper child;
};

struct DeleteEvent {
  std::unique_ptr<Widget> removed;
};

struct DisableEvent {
  WidgetWrapper receiver;
};

struct EnableEvent {
  WidgetWrapper receiver;
};

struct FocusInEvent {
  WidgetWrapper receiver;
};

struct FocusOutEvent {
  WidgetWrapper receiver;
};

struct MoveEvent {
  WidgetWrapper receiver;
  Point new_position;
};

struct ResizeEvent {
  WidgetWrapper receiver;
  Area new_area;
};

struct TimerEvent {
  WidgetWrapper receiver;
};

/** \p send will be called to send the event, typically would call on a member
 *  function of some receiving Widget type. \p filter_send should call whatever
 *  filter method on each installed filter, and return true if one of the
 *  filters handled the event. */
struct CustomEvent {
  std::function<void()> send;
  std::function<bool()> filter_send = [] { return false; };
};

using Event = absl::variant<PaintEvent,
                           KeyPressEvent,
                           MousePressEvent,
                           MouseReleaseEvent,
                           MouseDoubleClickEvent,
                           MouseWheelEvent,
                           MouseMoveEvent,
                           ChildAddedEvent,
                           ChildRemovedEvent,
                           ChildPolishedEvent,
                           DeleteEvent,
                           DisableEvent,
                           EnableEvent,
                           FocusInEvent,
                           FocusOutEvent,
                           MoveEvent,
                           ResizeEvent,
                           TimerEvent,
                           CustomEvent>;

}  // namespace npp
#endif  // NCURSESPLUSPLUS_SYSTEM_EVENT_HPP
