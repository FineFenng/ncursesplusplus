#include "ncursesplusplus/system/detail/user_input_event_loop.hpp"

#include <optional>
#include <utility>

#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/terminal/input.hpp"

namespace npp::detail {

void User_input_event_loop::loop_function() {
  if (auto event = input::get(); event != std::nullopt)
    System::post_event(std::move(*event));
}

}  // namespace npp::detail
