#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_USER_INPUT_EVENT_LOOP_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_USER_INPUT_EVENT_LOOP_HPP
#include "ncursesplusplus/system/event_loop.hpp"

namespace npp::detail {

/// Event loop that blocks for user input on each iteration.
/** Uses ncurses internally to get input. This is will also process the
 *  Event_queue and flush all changes to the screen on each iteration. */
class User_input_event_loop : public Event_loop {
   public:
    User_input_event_loop() { Event_loop::is_main_thread_ = true; }

   protected:
    /// Wait on input::get(), and post the result.
    void loop_function() override;
};

}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_USER_INPUT_EVENT_LOOP_HPP
