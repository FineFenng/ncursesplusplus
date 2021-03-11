#ifndef NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_COLORS_HPP
#define NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_COLORS_HPP
#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/painter/dynamic_colors.hpp"

namespace gol {
namespace color {

inline auto constexpr Black       = npp::Color::Background;
inline auto constexpr Light_green = npp::Color{1};
inline auto constexpr Green       = npp::Color{2};
inline auto constexpr Teal        = npp::Color{3};
inline auto constexpr Rainbow     = npp::Color{4};
inline auto constexpr White       = npp::Color::Foreground;

}  // namespace color

inline auto const gol_palette = npp::Palette{
    /* clang-format off */
        {color::Black,       npp::ANSI{16}, 0x0c2e44},
        {color::Light_green, npp::ANSI{17}, 0x5ac54f},
        {color::Green,       npp::ANSI{18}, 0x33984b},
        {color::Teal,        npp::ANSI{19}, 0x134c4c},
        {color::Rainbow,     npp::ANSI{20},
                                            npp::dynamic::rainbow(std::chrono::milliseconds{40}, 75, 70)},
        {color::White,       npp::ANSI{21}, 0xf9e6cf},
    /* clang-format on */
};

}  // namespace gol
#endif  // NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_COLORS_HPP
