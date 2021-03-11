#ifndef NCURSESPLUSPLUS_WIDGET_AREA_HPP
#define NCURSESPLUSPLUS_WIDGET_AREA_HPP
#include <cstddef>

namespace npp {

/// Convinience data structure holding Widget size dimensions.
struct Area {
    std::size_t width;
    std::size_t height;
};

/// Compares the dimension values, not the square areas.
inline auto operator==(Area const& x, Area const& y) -> bool
{
    return x.width == y.width && x.height == y.height;
}

/// Compares the dimension values, not the square areas.
inline auto operator!=(Area const& x, Area const& y) -> bool
{
    return !(x == y);
}

/// Compares the square area values, not the individual dimensions.
inline auto operator<(Area const& x, Area const& y) -> bool
{
    return (x.width * x.height) < (y.width * y.height);
}

}  // namespace npp
#endif  // NCURSESPLUSPLUS_WIDGET_AREA_HPP
