#ifndef NCURSESPLUSPLUS_SYSTEM_EVENT_FWD_HPP
#define NCURSESPLUSPLUS_SYSTEM_EVENT_FWD_HPP

#include "absl/types/variant.h"

namespace npp {

struct PaintEvent;
struct KeyPressEvent;
struct MousePressEvent;
struct MouseReleaseEvent;
struct MouseDoubleClickEvent;
struct MouseWheelEvent;
struct MouseMoveEvent;
struct ChildAddedEvent;
struct ChildRemovedEvent;
struct ChildPolishedEvent;
struct DeleteEvent;
struct DisableEvent;
struct EnableEvent;
struct FocusInEvent;
struct FocusOutEvent;
struct MoveEvent;
struct ResizeEvent;
struct TimerEvent;
struct CustomEvent;

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
#endif  // NCURSESPLUSPLUS_SYSTEM_EVENT_FWD_HPP
