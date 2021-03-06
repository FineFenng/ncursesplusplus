#ifndef NCURSESPLUSPLUS_TERMINAL_INPUT_HPP
#define NCURSESPLUSPLUS_TERMINAL_INPUT_HPP
#include <optional>

#include "ncursesplusplus/system/event.hpp"

namespace npp::input {

/// Wait for user input, and return with a corresponding Event.
/** Blocking call, input can be received from the keyboard, mouse, or the
 *  terminal being resized. Will return nullptr if there is an error. */
auto get() -> std::optional<Event>;

}  // namespace npp::input
#endif  // NCURSESPLUSPLUS_TERMINAL_INPUT_HPP
