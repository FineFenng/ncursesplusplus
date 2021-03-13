#include "ncursesplusplus/system/animation_engine.hpp"

#include <algorithm>
#include <iterator>
#include <memory>

#include "ncursesplusplus/system/detail/timer_event_loop.hpp"

namespace npp {

void AnimationEngine::RegisterWidget(Widget &w, Period_t interval) {
  if (!this->has_loop_with(interval)) {
    loops_.emplace_back(
        std::make_unique<detail::TimerEventLoop>(interval));
    loops_.back()->run_async();
  }
  this->get_loop_with(interval).RegisterWidget(w);
}

void AnimationEngine::RegisterWidget(Widget &w, FPS fps) {
  this->RegisterWidget(w, detail::IntervalEventLoop::FpsToPeriod(fps));
}

void AnimationEngine::UnregisterWidget(Widget &w) {
  // Unregister the Widget from the event loop it is contained in, and return
  // that event loop.
  auto const iter = std::find_if(
      std::begin(loops_), std::end(loops_),
      [&w](auto const &loop) { return loop->UnregisterWidget(w); });

  // If an event loop was found, and it is now empty, Shutdown that event loop
  if (iter != std::end(loops_)) {
    if (auto &loop = *iter; loop->IsEmpty()) {
      loop->Exit(0);
      loop->Wait();
      loops_.erase(iter);
    }
  }
}

void AnimationEngine::Shutdown() {
  /* Timer_event_loops will wait on the future at destruction.
   * Because Shutdown is called from EventLoop and will wait forever. */
  for (auto &loop : loops_) {
    loop->Exit(0);
    loop->Wait();
  }
}

void AnimationEngine::Startup() {
  for (auto &loop : loops_)
    loop->run_async();
}

}  // namespace npp
