#ifndef NCURSESPLUSPLUS_WIDGET_DETAIL_GRAPH_TREE_HPP
#define NCURSESPLUSPLUS_WIDGET_DETAIL_GRAPH_TREE_HPP
#include <string>

namespace npp {
class Widget;
}  // namespace npp

namespace npp::detail {

/// Outputs filename.gz graph description of widget tree hierarchy.
void graph_tree(Widget const &w, std::string const &filename);

}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_WIDGET_DETAIL_GRAPH_TREE_HPP
