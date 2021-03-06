#ifndef NCURSESPLUSPLUS_PAINTER_PALETTE_GAMEBOY_POCKET_HPP
#define NCURSESPLUSPLUS_PAINTER_PALETTE_GAMEBOY_POCKET_HPP
#include "ncursesplusplus/painter/color.hpp"

namespace npp::gameboy_pocket {

constexpr auto Black = Color::Background;
constexpr auto Dark_gray = Color{8};
constexpr auto Light_gray = Color{7};
constexpr auto White = Color::Foreground;

/// Gameboy Pocket Grayscale Color Palette
inline auto const palette = Palette{
    /* clang-format off */
    {Black, ANSI{16}, 0x000000},
    {Dark_gray, ANSI{17}, 0x545454},
    {Light_gray, ANSI{18}, 0xa9a9a9},
    {White, ANSI{19}, 0xffffff},
    /* clang-format on */
};

}  // namespace npp::gameboy_pocket
#endif  // NCURSESPLUSPLUS_PAINTER_PALETTE_GAMEBOY_POCKET_HPP
