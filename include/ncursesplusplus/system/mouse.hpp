#ifndef NCURSESPLUSPLUS_SYSTEM_MOUSE_HPP
#define NCURSESPLUSPLUS_SYSTEM_MOUSE_HPP
#include <cstdint>

#include "ncursesplusplus/widget/point.hpp"

namespace npp {

/// Mouse related data generated from Mouse Events.
struct Mouse {
  /// Standard mouse buttons to distinguish input.
  enum class Button : std::uint16_t {
    Left,
    Middle,
    Right,
    ScrollUp,
    ScrollDown
  };

  /// The terminal screen global coordinate of the input event.
  /** Top left of screen is (x:0, y:0). */
  Point global;

  /// The Widget local coordinate of the input event.
  /** Top left of Widget is (x:0, y:0). */
  Point local;

  /// The mouse button that created the event.
  Button button;

  /// Input device's ID.
  short device_id;

  /// Keyboard modifiers
  struct Modifiers {
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
  } modifiers;
};

}  // namespace npp
#endif  // NCURSESPLUSPLUS_SYSTEM_MOUSE_HPP
