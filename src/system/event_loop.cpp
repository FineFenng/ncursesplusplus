#include "ncursesplusplus/system/event_loop.hpp"

#include "ncursesplusplus/system/detail/event_engine.hpp"
#include "ncursesplusplus/system/system.hpp"

namespace npp {

auto EventLoop::run() -> int {
  if (running_)
    return -1;
  running_ = true;
  while (!exit_) {
    if (is_main_thread_)
      System::event_engine().process();
    this->LoopFunction();
  }
  running_ = false;
  exit_ = false;
  return return_code_;
}

void EventLoop::connect_to_system_exit() {
  auto exit_loop =
      sl::Slot < void(int) > {[this](int code) { this->Exit(code); }};
  exit_loop.track(lifetime_);
  System::exit_signal.connect(exit_loop);
}

}  // namespace npp
