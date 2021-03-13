#include "ncursesplusplus/system/detail/timer_event_loop.hpp"

#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/widget/widget.hpp"

namespace npp::detail {

void TimerEventLoop::RegisterWidget(Widget &w) {
  {
    auto const guard = Guard_t{mtx_registered_widgets_};
    registered_widgets_.emplace(&w);
  }
  w.destroyed.connect([this, &w] { this->UnregisterWidget(w); });
}

void TimerEventLoop::LoopFunction() {
  // The first call to this returns immediately.
  IntervalEventLoop::LoopFunction();
  {
    auto const guard = Guard_t{mtx_registered_widgets_};
    for (Widget *widg : registered_widgets_)
      System::post_event(Timer_event{*widg});
  }
}

}  // namespace npp::detail
