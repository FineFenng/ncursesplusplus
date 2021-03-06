#include "ncursesplusplus/widget/detail/border_offset.hpp"

#include <cstddef>

#include "ncursesplusplus/widget/border.hpp"
#include "ncursesplusplus/widget/widget.hpp"

namespace npp::detail {

auto BorderOffset::west_disqualified(Widget const &w) -> bool {
  return w.outer_width() <= 1;
}

auto BorderOffset::east_disqualified(Widget const &w) -> bool {
  if (w.outer_width() <= 1)
    return true;
  if (w.outer_width() == 2 and w.border.segments.west.enabled())
    return true;
  return false;
}

auto BorderOffset::north_disqualified(Widget const &w) -> bool {
  return w.outer_height() <= 1;
}

auto BorderOffset::south_disqualified(Widget const &w) -> bool {
  if (w.outer_height() <= 1)
    return true;
  if (w.outer_height() == 2 and w.border.segments.north.enabled())
    return true;
  return false;
}

auto BorderOffset::west(Widget const &w) -> std::size_t {
  return west_enabled(w) and !west_disqualified(w) ? 1 : 0;
}

auto BorderOffset::east(Widget const &w) -> std::size_t {
  return east_enabled(w) and !east_disqualified(w) ? 1 : 0;
}

auto BorderOffset::north(Widget const &w) -> std::size_t {
  return north_enabled(w) and !north_disqualified(w) ? 1 : 0;
}

auto BorderOffset::south(Widget const &w) -> std::size_t {
  return south_enabled(w) and !south_disqualified(w) ? 1 : 0;
}

auto BorderOffset::west_enabled(Widget const &w) -> bool {
  auto const &b = w.border.segments;
  return w.border.enabled() and (b.west.enabled() or b.north_west.enabled() or
      b.south_west.enabled());
}

auto BorderOffset::east_enabled(Widget const &w) -> bool {
  auto const &b = w.border.segments;
  return w.border.enabled() and (b.east.enabled() or b.north_east.enabled() or
      b.south_east.enabled());
}

auto BorderOffset::north_enabled(Widget const &w) -> bool {
  auto const &b = w.border.segments;
  return w.border.enabled() and
      (b.north.enabled() or b.north_east.enabled() or
          b.north_west.enabled());
}

auto BorderOffset::south_enabled(Widget const &w) -> bool {
  auto const &b = w.border.segments;
  return w.border.enabled() and
      (b.south.enabled() or b.south_east.enabled() or
          b.south_west.enabled());
}

}  // namespace npp::detail
