#ifndef TERMOX_DEMOS_PALETTE_PALETTE_HELPERS_HPP
#define TERMOX_DEMOS_PALETTE_PALETTE_HELPERS_HPP
#include <cstdint>

#include <termox/painter/color.hpp>
// #include <ncursesplusplus/painter/palette.hpp>

namespace palette {
namespace detail {

/// Return the current ANSI_definition of \p color in \p palette.
// npp::ANSI_definition& extract_definition(npp::Palette& palette,
//                                                npp::Color color);

/// Set the red value of \p color and update the display.
// void set_red(npp::Color color, npp::Underlying_color_t red_value);

// /// Set the green value of \p color and update the display.
// void set_green(npp::Color color, npp::Underlying_color_t
// green_value);

// /// Set the blue value of \p color and update the display.
// void set_blue(npp::Color color, npp::Underlying_color_t
// blue_value);

}  // namespace detail
}  // namespace palette
#endif  // TERMOX_DEMOS_PALETTE_PALETTE_HELPERS_HPP
