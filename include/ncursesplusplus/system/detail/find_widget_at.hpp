#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_FIND_WIDGET_AT_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_FIND_WIDGET_AT_HPP
#include "ncursesplusplus/widget/point.hpp"

namespace npp {
class Widget;
}  // namespace npp

namespace npp::detail {

/// Return the Widget* that owns the Point p in global coordinates.
/** Return nullptr on failing to find a Widget with the provided coordinates.
 *  Return the deepest child Widget that owns the coordinates. If a parent owns
 *  the coordinates, it is checked if any of the children own it as well before
 *  returning. Used only by input::get at the moment. */
auto find_widget_at(Point p) -> Widget *;

}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_FIND_WIDGET_AT_HPP
