#include "palette_helpers.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>

#include "ncursesplusplus/painter/color.hpp"
// #include <ncursesplusplus/painter/palette.hpp"
// #include <ncursesplusplus/painter/rgb.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/terminal/terminal.hpp"

using namespace npp;

namespace palette {
namespace detail {

// ANSI_definition& extract_definition(Palette& palette, Color color)
// {
//     auto at = std::find_if(
//         std::begin(palette), std::end(palette),
//         [color](const ANSI_definition& def) { return def.color == color; });
//     if (at != std::end(palette)) {
//         return *at;
//     }
//     return palette[0];
// }
}  // namespace detail
}  // namespace palette

namespace {
using namespace palette::detail;

/// Set a color definition for the terminal.
// void set_definition(Color color, RGB values)
// {
//     auto palette      = npp::System::terminal.current_palette();
//     auto& definition  = extract_definition(palette, color);
//     definition.values = values;
//     npp::System::terminal.set_color_palette(palette);
// }
}  // namespace

namespace palette {
namespace detail {

// void set_red(Color color, Underlying_color_t red_value)
// {
//     auto palette    = System::terminal.current_palette();
//     const auto& def = extract_definition(palette, color);
//     set_definition(color, {red_value, def.values.green, def.values.blue});
// }

// void set_green(Color color, Underlying_color_t green_value)
// {
//     auto palette    = System::terminal.current_palette();
//     const auto& def = extract_definition(palette, color);
//     set_definition(color, {def.values.red, green_value, def.values.blue});
// }

// void set_blue(Color color, Underlying_color_t blue_value)
// {
//     auto palette    = System::terminal.current_palette();
//     const auto& def = extract_definition(palette, color);
//     set_definition(color, {def.values.red, def.values.green, blue_value});
// }
}  // namespace detail
}  // namespace palette
