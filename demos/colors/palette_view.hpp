#ifndef NCURSESPLUSPLUS_DEMOS_COLORS_COLORS_DISPLAY_HPP
#define NCURSESPLUSPLUS_DEMOS_COLORS_COLORS_DISPLAY_HPP
#include <utility>

#include <signals_light/signal.hpp"

#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/painter/glyph_string.hpp"
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
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/widgets/color_select.hpp"
#include "ncursesplusplus/widget/widgets/cycle_box.hpp"

namespace colors {

class Palette_picker : public npp::Labeled_cycle_box {
   public:
    sl::Signal<void(npp::Palette)> palette_picked;

   public:
    Palette_picker() : Labeled_cycle_box{"Palette"}
    {
        using namespace npp;
        this->append_option(L"Dawn Bringer 16", dawn_bringer16::palette);
        this->append_option(L"Dawn Bringer 32", dawn_bringer32::palette);
        this->append_option(L"Basic 16", basic::palette);
        this->append_option(L"Basic 8", basic8::palette);
        this->append_option(L"EN 4", en4::palette);
        this->append_option(L"Stormy 6", stormy6::palette);
        this->append_option(L"Teletext", teletext::palette);
        this->append_option(L"Gameboy", gameboy::palette);
        this->append_option(L"Gameboy Pocket", gameboy_pocket::palette);
        this->append_option(L"NES", nes::palette);
        this->append_option(L"CGA", cga::palette);
        this->append_option(L"Amstrad CPC", amstrad_cpc::palette);
        this->append_option(L"Early MSFT Windows", windows::palette);
        this->append_option(L"Apple II", apple_ii::palette);
        this->append_option(L"Macintosh II", macintosh_ii::palette);
        this->append_option(L"MSX", msx::palette);
        this->append_option(L"ZX Spectrum", zx_spectrum::palette);
        this->append_option(L"Thomson M05", thomson_m05::palette);
        this->append_option(L"SECAM", secam::palette);
        this->append_option(L"Commodore VIC-20", commodore_vic20::palette);
        this->append_option(L"Commodore 64", commodore_64::palette);
        this->append_option(L"Windows Console", windows_console::palette);
        this->append_option(L"Atelier Savanna", savanna::palette);
        this->append_option(L"Ashes", ashes::palette);
    }

   private:
    /// Adds an entry to the cycle box for the given palette with \p title.
    void append_option(npp::Glyph_string title, npp::Palette const& pal)
    {
        this->cycle_box.add_option(std::move(title)).connect([this, pal]() {
            palette_picked(pal);
        });
    }
};

class Palette_demo : public npp::layout::Vertical<> {
   public:
    npp::Color_select& palette_view = this->make_child<npp::Color_select>(true);
    Palette_picker& palette_picker = this->make_child<Palette_picker>();

   public:
    Palette_demo()
    {
        using namespace npp::pipe;
        *this | strong_focus();
        palette_picker.palette_picked.connect(
            [](auto const& pal) { npp::System::terminal.set_palette(pal); });
    }
};

}  // namespace colors
#endif  // NCURSESPLUSPLUS_DEMOS_COLORS_COLORS_DISPLAY_HPP
