#ifndef NCURSESPLUSPLUS_DEMOS_PALETTE_COLOR_DISPLAY_HPP
#define NCURSESPLUSPLUS_DEMOS_PALETTE_COLOR_DISPLAY_HPP
#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/widget/layouts/horizontal.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/widget.hpp"
#include "ncursesplusplus/widget/widgets/text_display.hpp"

namespace palette {

/// Square visual display of a given Color.
struct Color_patch : npp::Widget {
    explicit Color_patch(npp::Color value)
    {
        this->set_color(value);
        this->width_policy.fixed(5);
        this->height_policy.fixed(2);
    }

    void set_color(npp::Color value)
    {
        this->brush.background = value;
        this->update();
    }
};

/// Title and name display of a given Color.
struct Color_name : npp::layout::Vertical<npp::Text_display> {
    npp::Text_display& title = this->MakeChild("Color:");
    npp::Text_display& name  = this->MakeChild("White");
};

/// Displays a text name and a square visual of a given Color.
struct Color_display : npp::layout::Horizontal<> {
    explicit Color_display(npp::Color color)
    {
        this->height_policy.fixed(3);
        this->border.enable();
        this->border.segments.disable_all();
        this->border.segments.south.enable();
        this->set_color(color);
    }

    /// Change the currently displayed color.
    void set_color(npp::Color color)
    {
        text.name.set_contents(color_to_string(color));
        square.set_color(color);
    }

    Color_name& text    = this->MakeChild<Color_name>();
    Color_patch& square = this->MakeChild<Color_patch>(npp::Color::White);
};
}  // namespace palette
#endif  // NCURSESPLUSPLUS_DEMOS_PALETTE_COLOR_DISPLAY_HPP
