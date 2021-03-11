#ifndef NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_PATTERNS_RULESETS_BOX_HPP
#define NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_PATTERNS_RULESETS_BOX_HPP
#include <string>
#include <utility>

#include <signals_light/signal.hpp"

#include "ncursesplusplus/painter/glyph_string.hpp"
#include "ncursesplusplus/widget/layouts/stack.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/widgets/button.hpp"
#include "ncursesplusplus/widget/widgets/button_list.hpp"

#include "colors.hpp"
#include "make_break.hpp"

namespace gol {

/// Custom Button.
class Thin_btn : public npp::Button {
   public:
    Thin_btn(npp::Glyph_string name) : Button{std::move(name)}
    {
        using namespace npp::pipe;
        *this | fixed_height(1uL) | npp::bg(color::Light_green) |
        npp::fg(color::Teal);
    }
};

/// A Button_list, line break, and goto Button in Vertical layout.
class Selection_page : public npp::layout::Vertical<> {
   private:
    using Selection_list = npp::Button_list<npp::layout::Vertical>;

   public:
    Selection_list& list = this->make_child<Selection_list>();
    Thin_btn& goto_btn;

   public:
    Selection_page(npp::Glyph_string goto_btn_name)
        : goto_btn{this->make_child<Thin_btn>(std::move(goto_btn_name))}
    {
        list.set_scrollbar_bg(color::Teal);
        list.set_scrollbar_fg(color::Light_green);
    }

   public:
    sl::Signal<void(std::wstring const& name)>& selection_made =
        list.button_pressed;

   public:
    void add_option(std::wstring const& name) { list.add_button(name); }
};

/// Stack of patterns and rules pages.
class Patterns_rulesets_box : public npp::layout::Stack<Selection_page> {
   public:
    // The String is for the GoTo Button
    Selection_page& patterns = this->make_page(L"Rulesets");
    Selection_page& rulesets = this->make_page(L"Patterns");

   public:
    Patterns_rulesets_box()
    {
        using namespace npp::pipe;
        *this | active_page(patterns_index) | fixed_height(10);

        patterns.goto_btn |
            on_press([this]() { this->set_active_page(rulesets_index); });

        rulesets.goto_btn |
            on_press([this]() { this->set_active_page(patterns_index); });
    }

   private:
    static auto constexpr patterns_index = 0uL;
    static auto constexpr rulesets_index = 1uL;
};

}  // namespace gol
#endif  // NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_PATTERNS_RULESETS_BOX_HPP
