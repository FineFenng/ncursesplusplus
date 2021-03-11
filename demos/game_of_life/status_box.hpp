#ifndef NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_STATUS_BOX_HPP
#define NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_STATUS_BOX_HPP
#include <cstdint>

#include "ncursesplusplus/painter/glyph_string.hpp"
#include "ncursesplusplus/widget/align.hpp"
#include "ncursesplusplus/widget/layouts/horizontal.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/widgets/label.hpp"
#include "ncursesplusplus/widget/widgets/line_edit.hpp"
#include "ncursesplusplus/widget/widgets/number_edit.hpp"
#include "ncursesplusplus/widget/widgets/text_display.hpp"

#include "make_break.hpp"

namespace gol {

class Generation_count : public npp::layout::Horizontal<> {
   public:
    Generation_count()
    {
        this->height_policy.fixed(1);
        this->cursor.disable();
        title_ | npp::pipe::fixed_width(11uL);
        count_ | npp::pipe::fixed_width(5uL);
    }

    void update_count(std::uint32_t count)
    {
        count_.set_contents(std::to_string(count));
    }

   private:
    npp::HLabel& title_ = this->make_child<npp::HLabel>({L"Generation"});

    npp::Text_display& count_ = this->make_child<npp::Text_display>(L"0");
};

struct Center_offset : npp::layout::Vertical<> {
   public:
    npp::HLabel& title_ =
        this->make_child<npp::HLabel>({L"Center Offset" | npp::Trait::Underline});

    npp::Labeled_number_edit<>& x_coords =
        this->make_child<npp::Labeled_number_edit<>>(L"x: ", 0);

    npp::Labeled_number_edit<>& y_coords =
        this->make_child<npp::Labeled_number_edit<>>(L"y: ", 0);

   public:
    Center_offset()
    {
        using namespace npp;

        title_.set_alignment(Align::Center);

        x_coords.number_edit.brush.background = color::Black;
        x_coords.number_edit.brush.foreground = color::White;
        x_coords.number_edit.set_ghost_color(color::White);

        y_coords.number_edit.brush.background = color::Black;
        y_coords.number_edit.brush.foreground = color::White;
        y_coords.number_edit.set_ghost_color(color::White);
    }
};

struct Status_box : npp::layout::Vertical<> {
   public:
    Generation_count& gen_count  = this->make_child<Generation_count>();
    Widget& break_               = this->append_child(make_break());
    Center_offset& center_offset = this->make_child<Center_offset>();

   public:
    Status_box() { this->height_policy.fixed(5uL); }
};
}  // namespace gol
#endif  // NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_STATUS_BOX_HPP
