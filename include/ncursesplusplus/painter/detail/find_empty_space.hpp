#ifndef NCURSESPLUSPLUS_PAINTER_DETAIL_FIND_EMPTY_SPACE_HPP
#define NCURSESPLUSPLUS_PAINTER_DETAIL_FIND_EMPTY_SPACE_HPP
#include "ncursesplusplus/painter/detail/screen_mask.hpp"

namespace npp {
class Widget;
}  // namespace npp

namespace npp {
namespace detail {

/// Return a  { namespace detailask representing empty space for the Widget \p w.
/** Where a set bit means you have no enabled child owning that point. Used to
 *  find where a Layout should paint wallpaper tiles. */
auto find_empty_space(Widget const &w) -> Screen_mask;

}
}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_PAINTER_DETAIL_FIND_EMPTY_SPACE_HPP
}