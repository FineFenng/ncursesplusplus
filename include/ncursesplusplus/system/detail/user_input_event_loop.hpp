#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_USER_INPUT_EVENT_LOOP_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_USER_INPUT_EVENT_LOOP_HPP
#include "ncursesplusplus/system/event_loop.hpp"

namespace npp {
namespace detail {

/// Event loop that blocks for user input on each iteration.
/** Uses ncurses internally to get input. This is will also process the
 *  EventQueue and flush all changes to the screen on each iteration. */
class UserInputEventLoop : public EventLoop {
 public:
  UserInputEventLoop() { EventLoop::is_main_thread_ = true; }
  ~UserInputEventLoop() override = default;

 protected:
  /// Wait on input::get(), and post the result.
  void LoopFunction() override;
};

}
}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_USER_INPUT_EVENT_LOOP_HPP
