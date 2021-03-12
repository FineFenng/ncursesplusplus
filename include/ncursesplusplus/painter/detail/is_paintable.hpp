#ifndef NCURSESPLUSPLUS_PAINTER_DETAIL_IS_PAINTABLE_HPP
#define NCURSESPLUSPLUS_PAINTER_DETAIL_IS_PAINTABLE_HPP
#include "ncursesplusplus/widget/widget.hpp"

namespace npp::detail {

/// A check for whether a widget is in a state that can be painted.
inline auto is_paintable(Widget const &w) -> bool {
  return w.is_enabled() and (w.outer_width() != 0) and
      (w.outer_height() != 0);
}

}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_PAINTER_DETAIL_IS_PAINTABLE_HPP
