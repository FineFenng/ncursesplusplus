#ifndef TERMOX_WIDGET_DETAIL_GRAPH_TREE_HPP
#define TERMOX_WIDGET_DETAIL_GRAPH_TREE_HPP
#include <string>

namespace npp {
class Widget;
}  // namespace npp

namespace npp::detail {

/// Outputs filename.gz graph description of widget tree hierarchy.
void graph_tree(Widget const& w, std::string const& filename);

}  // namespace npp::detail
#endif  // TERMOX_WIDGET_DETAIL_GRAPH_TREE_HPP
