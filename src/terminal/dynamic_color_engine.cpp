#include "ncursesplusplus/terminal/dynamic_color_engine.hpp"

#include <mutex>
#include <utility>
#include <vector>

#include "ncursesplusplus/painter/detail/screen.hpp"
#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/terminal/output.hpp"

namespace npp::detail {

namespace {

using Processed_colors = std::vector<std::pair<ANSI, True_color>>;

/// Create a CustomEvent to update color definitions.
auto dynamic_color_event(Processed_colors colors) -> CustomEvent {
  return {[=] {
    {
      for (auto&[ansi, true_color] : colors)
        System::terminal.term_set_color(ansi, true_color);
    }
    npp::output::refresh();
  }};
}

}  // namespace

void Dynamic_color_event_loop::LoopFunction() {
  {
    auto processed = Processed_colors{};
    auto const guard = std::scoped_lock{colors_mtx_};
    for (auto&[ansi, dynamic] : colors_)
      processed.push_back({ansi, dynamic.get_value()});

    System::post_event(dynamic_color_event(processed));
  }
  IntervalEventLoop::LoopFunction();
}

}  // namespace npp::detail
