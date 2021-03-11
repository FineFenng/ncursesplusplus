#ifndef NCURSESPLUSPLUS_DEMOS_PALETTE_COLOR_DISPLAY_PANEL_HPP
#define NCURSESPLUSPLUS_DEMOS_PALETTE_COLOR_DISPLAY_PANEL_HPP
#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/widget/layouts/horizontal.hpp"
#include "ncursesplusplus/widget/widget.hpp"
#include "ncursesplusplus/widget/widgets/color_select.hpp"

namespace palette {

/// Displays every light and medium shade of a particular base color.
class Shade_display : public npp::Widget {
   public:
    explicit Shade_display(npp::Color base) : base_{base} {}

    /// Paint the shades from the top to the bottom.
    void invert() { inverted_ = true; }

   protected:
    bool paint_event() override;

   private:
    npp::Color base_;
    bool inverted_{false};
};

/// Top bank of shades to display, the first 8 colors.
struct Top_shades : npp::layout::Horizontal<Shade_display> {
    Shade_display& black_shade{this->make_child(npp::Color::Black)};
    Shade_display& dark_red_shade{this->make_child(npp::Color::Dark_red)};
    Shade_display& dark_blue_shade{this->make_child(npp::Color::Dark_blue)};
    Shade_display& dark_gray_shade{this->make_child(npp::Color::Dark_gray)};
    Shade_display& brown_shade{this->make_child(npp::Color::Brown)};
    Shade_display& green_shade{this->make_child(npp::Color::Green)};
    Shade_display& red_shade{this->make_child(npp::Color::Red)};
    Shade_display& gray_shade{this->make_child(npp::Color::Gray)};
};

/// Bottom bank of shades to display, the last 8 colors.
struct Bottom_shades : npp::layout::Horizontal<Shade_display> {
    Bottom_shades();

    Shade_display& blue_shade{this->make_child(npp::Color::Blue)};
    Shade_display& orange_shade{this->make_child(npp::Color::Orange)};
    Shade_display& light_gray_shade{this->make_child(npp::Color::Light_gray)};
    Shade_display& light_green_shade{this->make_child(npp::Color::Light_green)};
    Shade_display& violet_shade{this->make_child(npp::Color::Violet)};
    Shade_display& light_blue_shade{this->make_child(npp::Color::Light_blue)};
    Shade_display& yellow_shade{this->make_child(npp::Color::Yellow)};
    Shade_display& white_shade{this->make_child(npp::Color::White)};
};

/// Holds both the pure colors and the various shades.
struct All_colors_display : npp::layout::Vertical<> {
    All_colors_display();

    Top_shades& top_shades         = make_child<Top_shades>();
    npp::Color_select& color_select = make_child<npp::Color_select>();
    Bottom_shades& bottom_shades   = make_child<Bottom_shades>();
};

}  // namespace palette
#endif  // NCURSESPLUSPLUS_DEMOS_PALETTE_COLOR_DISPLAY_PANEL_HPP
