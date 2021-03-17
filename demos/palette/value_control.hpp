#ifndef NCURSESPLUSPLUS_DEMOS_PALETTE_VALUE_CONTROL_HPP
#define NCURSESPLUSPLUS_DEMOS_PALETTE_VALUE_CONTROL_HPP
#include <signals_light/signal.hpp"

#include "ncursesplusplus/painter/glyph_string.hpp"
#include "ncursesplusplus/widget/layouts/horizontal.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/widgets/label.hpp"
#include "ncursesplusplus/widget/widgets/line_edit.hpp"
#include "ncursesplusplus/widget/widgets/vertical_slider.hpp"

namespace palette::detail {

/// Vertical_slider with extra space around the sides.
struct Buffered_slider : npp::layout::Horizontal<> {
    using Slider_t = npp::Vertical_slider;

    Widget& left     = this->MakeChild() | npp::pipe::fixed_width(1);
    Slider_t& slider = this->MakeChild<Slider_t>(0, 255);
    Widget& right    = this->MakeChild() | npp::pipe::fixed_width(1);
};

/// Line_edit with extra space on the left side.
class Buffered_edit_box : public npp::layout::Horizontal<> {
   public:
    Buffered_edit_box();

   public:
    Widget& left       = this->MakeChild() | npp::pipe::fixed_width(1);
    npp::Line_edit& box = this->MakeChild<npp::Line_edit>("0");
};
}  // namespace palette::detail

namespace palette {

/// Provides user interface to change a color value from [0, 255].
/** Emits signal when changed. */
class Value_control : public npp::layout::Vertical<> {
    detail::Buffered_slider& slider_{
        this->MakeChild<detail::Buffered_slider>()};

    detail::Buffered_edit_box& value_edit_{
        this->MakeChild<detail::Buffered_edit_box>()};

    npp::HLabel& label_;

   public:
    explicit Value_control(const npp::Glyph_string& label);

    /// Set the current value being held.
    void set_value(int value) { slider_.slider.set_value(value); }

    sl::Signal<void(int)>& value_changed{slider_.slider.value_changed};
};

}  // namespace palette
#endif  // NCURSESPLUSPLUS_DEMOS_PALETTE_VALUE_CONTROL_HPP
