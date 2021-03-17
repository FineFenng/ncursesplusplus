#ifndef NCURSESPLUSPLUS_DEMOS_PALETTE_COLOR_DEFINITION_SETTER_HPP
#define NCURSESPLUSPLUS_DEMOS_PALETTE_COLOR_DEFINITION_SETTER_HPP
#include <cctype>
#include <string>

#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/painter/painter.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/widgets/confirm_button.hpp"
#include "ncursesplusplus/widget/widgets/line_edit.hpp"
#include "ncursesplusplus/widget/widgets/text_display.hpp"

#include "color_control.hpp"
#include "color_display.hpp"
#include "export_panel.hpp"

namespace palette {

/// Provides interface and implementation of setting a specific color value.
class Color_definition_setter : public npp::layout::Vertical<> {
    npp::Color current_color_{npp::Color::White};

    Color_display& color_display_{
        this->MakeChild<Color_display>(current_color_)};

    Color_control& color_control_{this->MakeChild<Color_control>()};

    Export_panel& export_panel_{this->MakeChild<Export_panel>()};

   public:
    Color_definition_setter();

    /// Change the color you are setting the definition of.
    void change_current_color(npp::Color color);

    /// Return the color you are setting the definition of.
    auto current_color() const -> npp::Color { return current_color_; }
};

}  // namespace palette
#endif  // NCURSESPLUSPLUS_DEMOS_PALETTE_COLOR_DEFINITION_SETTER_HPP
