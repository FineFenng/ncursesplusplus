#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_TIMER_EVENT_LOOP_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_TIMER_EVENT_LOOP_HPP
#include <chrono>
#include <functional>
#include <mutex>
#include <set>

#include "ncursesplusplus/system/detail/interval_event_loop.hpp"

#include "absl/synchronization/mutex.h"

namespace npp {
class Widget;
}  // namespace npp

namespace npp {
namespace detail {

/// Event loop for sending Timer_events at a given interval.
class TimerEventLoop : public IntervalEventLoop {
 private:
  using Mutex = absl::Mutex;
  using Guard = absl::MutexLock;

 public:
  ~TimerEventLoop() override {
    // Have to end the thread here, since it uses the std::set<...>
    Exit(0);
    Wait();
  }

 public:
  using IntervalEventLoop::IntervalEventLoop;

  /// Register a widget to have a TimerEvent posted to it every period.
  /** No-op if widget is already registered. */
  void RegisterWidget(Widget &w);

  /// Stop a widget from receiving Timer_events for this loop.
  /** Returns true if \p w was found and unregistered. */
  auto UnregisterWidget(Widget &w) -> bool {
    absl::MutexLock ml(&mtx_registered_widgets_);
    return registered_widgets_.erase(&w) == 1;
  }

  /// Remove all registered Widgets from this Event Loop.
  void Clear() {
    absl::MutexLock ml(&mtx_registered_widgets_);
    registered_widgets_.clear();
  }

  /// Return true if no Widgets are registered with this event loop.
  auto IsEmpty() const -> bool {
    absl::MutexLock ml(&mtx_registered_widgets_);
    return registered_widgets_.empty();
  }

 protected:
  void LoopFunction() override;

 private:
  std::set<Widget *> registered_widgets_;
  mutable absl::Mutex mtx_registered_widgets_;
};

}
}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_TIMER_EVENT_LOOP_HPP
