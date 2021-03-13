#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_INTERVAL_EVENT_LOOP_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_INTERVAL_EVENT_LOOP_HPP
#include <algorithm>
#include <chrono>
#include <thread>

#include "ncursesplusplus/system/event_loop.hpp"

namespace npp {

/// Frames Per Second
struct FPS {
  unsigned int value;
};

}  // namespace npp

namespace npp {
namespace detail {

/// LoopFunction() waits for a given time interval, then returns.
/** Not useful on its own, but can be inherited from, and its LoopFunction can
 *  be called on from the derived classes LoopFunction to perform the wait.
 *  Accounts for time since last wait so interval is not dependent on
 *  calculations in between the calls to wait. */
class IntervalEventLoop : public EventLoop {
 public:
  using Period = std::chrono::milliseconds;

 public:
  IntervalEventLoop() = default;
  explicit IntervalEventLoop(Period interval)
       : interval_{interval} {}

  ~ IntervalEventLoop() override = default;

  explicit IntervalEventLoop(FPS fps) : interval_{FpsToPeriod(fps)} {}

  Period Interval() const  { return interval_; }

 public:
  /// Converts frames per second to an interval of time.
  static Period FpsToPeriod(FPS fps) {
    return Period{static_cast<Period::rep>((1. / fps.value) *
        Period::period::den)};
  }

 protected:
  /// Tells the event engine to not processes the event queue.
  /** The first call to this method returns immediately. */
  void LoopFunction() override {
    auto leftovers = this->GetActualInterval();
    while (leftovers > exit_check_interval_) {
      if (IsExit()) {
        return;
      }
      std::this_thread::sleep_for(exit_check_interval_);
      leftovers -= exit_check_interval_;
    }
    std::this_thread::sleep_for(leftovers);
    last_time_ = Clock::now();  // Don't have to update if you are exiting
  }

 private:
  using Clock = std::chrono::high_resolution_clock;

  const Period  interval_;
  std::chrono::time_point<Clock> last_time_;
  static constexpr auto exit_check_interval_ = Period{30};

 private:
  /// Return the interval, minus the time since the last loop finished.
  /** Returns zero if the time elapsed since the last loop is larger than the
   *  set interval. Starting, then stopping, then starting again, will do the
   *  right thing. Returns zero on the first call. */
  Clock::duration GetActualInterval() const {
    auto const elapsed = Clock::now() - last_time_;
    return std::max(Clock::duration::zero(), interval_ - elapsed);
  }
};

}
}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_INTERVAL_EVENT_LOOP_HPP
