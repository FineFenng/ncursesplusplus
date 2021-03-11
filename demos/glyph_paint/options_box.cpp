#include "options_box.hpp"

#include <array>
#include <utility>

#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/painter/palette/amstrad_cpc.hpp"
#include "ncursesplusplus/painter/palette/apple_ii.hpp"
#include "ncursesplusplus/painter/palette/ashes.hpp"
#include "ncursesplusplus/painter/palette/basic.hpp"
#include "ncursesplusplus/painter/palette/basic8.hpp"
#include "ncursesplusplus/painter/palette/cga.hpp"
#include "ncursesplusplus/painter/palette/commodore_64.hpp"
#include "ncursesplusplus/painter/palette/commodore_vic20.hpp"
#include "ncursesplusplus/painter/palette/dawn_bringer16.hpp"
#include "ncursesplusplus/painter/palette/dawn_bringer32.hpp"
#include "ncursesplusplus/painter/palette/en4.hpp"
#include "ncursesplusplus/painter/palette/gameboy.hpp"
#include "ncursesplusplus/painter/palette/gameboy_pocket.hpp"
#include "ncursesplusplus/painter/palette/macintosh_ii.hpp"
#include "ncursesplusplus/painter/palette/msx.hpp"
#include "ncursesplusplus/painter/palette/nes.hpp"
#include "ncursesplusplus/painter/palette/savanna.hpp"
#include "ncursesplusplus/painter/palette/secam.hpp"
#include "ncursesplusplus/painter/palette/stormy6.hpp"
#include "ncursesplusplus/painter/palette/teletext.hpp"
#include "ncursesplusplus/painter/palette/thomson_m05.hpp"
#include "ncursesplusplus/painter/palette/windows.hpp"
#include "ncursesplusplus/painter/palette/windows_console.hpp"
#include "ncursesplusplus/painter/palette/zx_spectrum.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/terminal/terminal.hpp"
#include "ncursesplusplus/widget/pipe.hpp"

namespace paint {

Options_B::Options_B()
{
    using namespace npp;
    using namespace npp::pipe;

    space1 | wallpaper(L'─');
    palette_label | align_center();
    back_btn | bg(Color::Light_gray) | fg(Color::Black);
    palette_box | bg(Color::White) | fg(Color::Black);

    auto const palettes = std::array{
        std::pair{L"Dawn Bringer 16", dawn_bringer16::palette},
        std::pair{L"Dawn Bringer 32", dawn_bringer32::palette},
        std::pair{L"Basic 16", basic::palette},
        std::pair{L"Basic 8", basic8::palette},
        std::pair{L"EN 4", en4::palette},
        std::pair{L"Stormy 6", stormy6::palette},
        std::pair{L"Savanna VIM", savanna::palette},
        std::pair{L"Ashes VIM", ashes::palette},
        std::pair{L"Amstrad CPC", amstrad_cpc::palette},
        std::pair{L"Apple II", apple_ii::palette},
        std::pair{L"Macintosh II", macintosh_ii::palette},
        std::pair{L"CGA", cga::palette},
        std::pair{L"MSX", msx::palette},
        std::pair{L"SECAM", secam::palette},
        std::pair{L"Commodore 64", commodore_64::palette},
        std::pair{L"Commodore VIC-20", commodore_vic20::palette},
        std::pair{L"Gameboy", gameboy::palette},
        std::pair{L"Gameboy Pocket", gameboy_pocket::palette},
        std::pair{L"NES", nes::palette},
        std::pair{L"Teletext", teletext::palette},
        std::pair{L"Thomson M05", thomson_m05::palette},
        std::pair{L"Early Windows", windows::palette},
        std::pair{L"Windows Console", windows_console::palette},
        std::pair{L"ZX Spectrum", zx_spectrum::palette},
    };

    for (auto const& [name, palette] : palettes) {
        palette_box.add_option(name).connect(
            [p = palette] { System::terminal.set_palette(p); });
    }
}

}  // namespace paint
