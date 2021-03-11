#ifndef NCURSESPLUSPLUS_WIDGET_GROWTH_HPP
#define NCURSESPLUSPLUS_WIDGET_GROWTH_HPP
namespace npp {

/// Classifies ways a Widget might change size based on its contents.
enum class Growth : bool {
    Static,  // Fixed size, not dependent on contents
    Dynamic  // Grows in some proportion to the contents
};

}  // namespace npp
#endif  // NCURSESPLUSPLUS_WIDGET_GROWTH_HPP
