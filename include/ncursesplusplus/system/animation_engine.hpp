#ifndef NCURSESPLUSPLUS_SYSTEM_ANIMATION_ENGINE_HPP
#define NCURSESPLUSPLUS_SYSTEM_ANIMATION_ENGINE_HPP
#include <memory>
#include <vector>

#include "ncursesplusplus/system/detail/timer_event_loop.hpp"

namespace npp {
class Widget;

/// Manages all Timer_event_loops, grouping by period.
class AnimationEngine {
 public:
  using Period = detail::TimerEventLoop::Period;

 public:
  ~AnimationEngine() {
    for (auto &loop : loops_) {
      loop->Exit(0);
      loop->Wait();
    }
  }

 public:
  /// Begins posting Timer_events to the given Widget every period.
  void RegisterWidget(Widget &w, Period interval);

  /// Begins posting Timer_events to the given Widget at \p fps.
  void RegisterWidget(Widget &w, FPS fps);

  /// Stop posting Timer_events to a given Widget.
  void UnregisterWidget(Widget &w);

  // Start sending Timer_events to all registered widgets.
  /** Only needed if Shutdown() has been called. */
  void Startup();

  /// Send stop signals to all event loops and wait for them to exit.
  void Shutdown();

 private:
  // Using a std::unique_ptr because Event_loops can't be copied.
  using LoopPtr = std::unique_ptr<detail::TimerEventLoop>;
  std::vector<LoopPtr> loops_;
  using ConstLoopIter = std::vector<LoopPtr>::const_iterator;

 private:
  /// Find and return iterator pointing to Event Loop with \p interval.
  /** Returns std::end(loops_) if no loop found with \p interval. */
  ConstLoopIter GetLoopIterWith(Period interval) const {
    return std::find_if(loops_.begin(), loops_.end(),
                        [interval](const LoopPtr& loop_ptr) {
                          return loop_ptr->Interval() == interval;
                        });
  }

  /// Return true if there is already an EventLoop for \p interval.
  auto has_loop_with(Period interval) const -> bool {
    return GetLoopIterWith(interval) != loops_.end();
  }

  /// Assumes that the Event Loop does exist, otherwise undefined behavior.
  detail::TimerEventLoop& get_loop_with(Period interval) {
    return **GetLoopIterWith(interval);
  }
};

}  // namespace npp
#endif  // NCURSESPLUSPLUS_SYSTEM_ANIMATION_ENGINE_HPP
