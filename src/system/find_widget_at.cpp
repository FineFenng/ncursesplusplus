#include <termox/system/detail/find_widget_at.hpp>

#include <termox/system/system.hpp>
#include <termox/widget/point.hpp>
#include <termox/widget/widget.hpp>

namespace {

auto contains(npp::Widget const& w, npp::Point global) -> bool
{
    bool const within_west  = global.x >= w.inner_x();
    bool const within_east  = global.x < (w.inner_x() + w.width());
    bool const within_north = global.y >= w.inner_y();
    bool const within_south = global.y < (w.inner_y() + w.height());
    return within_west && within_east && within_north && within_south;
}

/// Returns a descendant of w that owns \p p, or nullptr if none found.
auto find_owner_of(npp::Widget& w, npp::Point p) -> npp::Widget*
{
    if (!w.is_enabled() || !contains(w, p))
        return nullptr;
    for (auto& child : w.get_children()) {
        if (npp::Widget* owner = find_owner_of(child, p); owner != nullptr)
            return owner;
    }
    return &w;
}

}  // namespace

namespace npp::detail {

auto find_widget_at(Point p) -> Widget*
{
    if (auto* head = System::head(); head == nullptr)
        return nullptr;
    else
        return find_owner_of(*head, p);
}

}  // namespace npp::detail
