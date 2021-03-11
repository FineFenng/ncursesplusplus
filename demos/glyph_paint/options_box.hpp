#ifndef DEMOS_GLYPH_PAINT_OPTIONS_BOX_HPP
#define DEMOS_GLYPH_PAINT_OPTIONS_BOX_HPP
#include <termox/painter/color.hpp>
#include <termox/widget/layouts/stack.hpp>
#include <termox/widget/layouts/vertical.hpp>
#include <termox/widget/pipe.hpp>
#include <termox/widget/widgets/button.hpp>
#include <termox/widget/widgets/checkbox.hpp>
#include <termox/widget/widgets/confirm_button.hpp>
#include <termox/widget/widgets/cycle_box.hpp>
#include <termox/widget/widgets/label.hpp>
#include <termox/widget/widgets/open_file.hpp>
#include <termox/widget/widgets/save_file.hpp>

namespace paint {

class Options_A : public npp::layout::Vertical<> {
   private:
    using Checkbox = npp::HCheckbox_label;

   public:
    npp::Button& clone_btn = make_child<npp::Button>(L"Clone Tool");
    Checkbox& eraser_box  = make_child<Checkbox>({L"Eraser"});
    Checkbox& cursor_box  = make_child<Checkbox>({L"Show Cursor"});
    Checkbox& grid_box    = make_child<Checkbox>({L"Show Grid"});
    npp::Confirm_button& clear_btn =
        make_child<npp::Confirm_button>(L"Clear Painting");
    npp::Button& more_btn = make_child<npp::Button>(L"More Options");

   public:
    Options_A()
    {
        using namespace npp::pipe;
        using npp::Color;
        clone_btn | bg(Color::White) | fg(Color::Black);
        more_btn | bg(Color::Light_gray) | fg(Color::Black);
        clear_btn.main_btn | bg(Color::White) | fg(Color::Black);
    }
};

class Options_B : public npp::layout::Vertical<> {
   public:
    npp::HLabel& palette_label  = make_child<npp::HLabel>({L"Color  Palette"});
    npp::Cycle_box& palette_box = make_child<npp::Cycle_box>();
    Widget& space1             = make_child() | npp::pipe::fixed_height(1);
    npp::Save_file<>& save_file = make_child<npp::Save_file<>>();
    npp::Open_file<>& open_file = make_child<npp::Open_file<>>();
    npp::Button& back_btn       = make_child<npp::Button>("Back");

   public:
    Options_B();
};

class Options_stack : public npp::layout::Stack<> {
   public:
    Options_A& options_a = make_child<Options_A>();
    Options_B& options_b = make_child<Options_B>();

   public:
    Options_stack()
    {
        using namespace npp::pipe;

        *this | active_page(0) | fixed_height(6);
        options_a.more_btn |
            on_press(npp::slot::set_active_page(*this, b_index_));
        options_b.back_btn |
            on_press(npp::slot::set_active_page(*this, a_index_));
    }

   private:
    static auto constexpr a_index_ = 0;
    static auto constexpr b_index_ = 1;
};

}  // namespace paint
#endif  // DEMOS_GLYPH_PAINT_OPTIONS_BOX_HPP
