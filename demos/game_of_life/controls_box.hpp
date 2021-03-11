#ifndef NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_SETTINGS_BOX_HPP
#define NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_SETTINGS_BOX_HPP
#include <cctype>
#include <chrono>
#include <string>

#include <signals_light/signal.hpp"

#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/painter/glyph.hpp"
#include "ncursesplusplus/painter/glyph_string.hpp"
#include "ncursesplusplus/painter/trait.hpp"
#include "ncursesplusplus/widget/align.hpp"
#include "ncursesplusplus/widget/layouts/horizontal.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/widget.hpp"
#include "ncursesplusplus/widget/widgets/button.hpp"
#include "ncursesplusplus/widget/widgets/checkbox.hpp"
#include "ncursesplusplus/widget/widgets/confirm_button.hpp"
#include "ncursesplusplus/widget/widgets/label.hpp"
#include "ncursesplusplus/widget/widgets/line_edit.hpp"
#include "ncursesplusplus/widget/widgets/number_edit.hpp"
#include "ncursesplusplus/widget/widgets/text_display.hpp"
#include "ncursesplusplus/widget/widgets/toggle_button.hpp"

#include "colors.hpp"
#include "make_break.hpp"
#include "rule.hpp"

namespace gol {

struct Clear_step_box : npp::layout::Vertical<> {
   public:
    npp::Button& step_btn = this->make_child<npp::Button>("Step>");

    npp::Confirm_button& clear_btn =
        this->make_child<npp::Confirm_button>("Clear");

   public:
    Clear_step_box()
    {
        using namespace npp;
        using namespace npp::pipe;

        *this | fixed_height(2uL);
        clear_btn.main_btn | bg(color::Light_green) | fg(color::Teal);
        clear_btn.confirm_page.confirm_btn | bg(color::Light_green) |
            fg(color::Teal);
        clear_btn.confirm_page.exit_btn | bg(color::Teal) | fg(color::White);
        step_btn | bg(color::Teal) | fg(color::Light_green);
    }
};

class Rule_edit : public npp::layout::Vertical<> {
   public:
    sl::Signal<void(std::string const&)> rule_change;

   public:
    Rule_edit()
    {
        using namespace npp;
        using namespace npp::pipe;

        *this | fixed_height(2uL);
        edit_box_ | bg(color::White) | fg(color::Black) | ghost(color::Teal);

        edit_box_.set_validator([](char c) {
            return std::isdigit(c) || c == '/' || c == 'B' || c == 'b' ||
                   c == 'S' || c == 's';
        });

        edit_box_.edit_finished.connect(
            [this](std::string rule_text) { rule_change(rule_text); });

        this->set_rule(parse_rule_string("B3/S23"));
    }

   public:
    void set_rule(Rule r)
    {
        auto const rs = to_rule_string(r);
        edit_box_.set_contents(rs);
        edit_box_.set_cursor(rs.size());
    }

   private:
    npp::HLabel& label = this->make_child<npp::HLabel>(
        {L"RuleString[B/S]" | npp::Trait::Underline | npp::Trait::Bold,
         npp::Align::Center});

    npp::Line_edit& edit_box_ = this->make_child<npp::Line_edit>();
};

struct Start_pause_btns : npp::Toggle_button {
   public:
    sl::Signal<void()>& start_requested = top.pressed;
    sl::Signal<void()>& pause_requested = bottom.pressed;

   public:
    Start_pause_btns() : Toggle_button{L"Start", L"Pause"}
    {
        using namespace npp;
        using namespace npp::pipe;

        *this | fixed_height(1uL);
        top | bg(color::Light_green) | fg(color::Teal);
        bottom | bg(color::Green) | fg(color::White);
    }
};

struct Interval_box : npp::layout::Horizontal<> {
   public:
    npp::Labeled_number_edit<unsigned>& value_edit =
        this->make_child<npp::Labeled_number_edit<unsigned>>("Interval ", 40);

    npp::HLabel& units = this->make_child<npp::HLabel>({L"ms"});

    sl::Signal<void(unsigned)>& value_set = value_edit.value_set;

   public:
    Interval_box()
    {
        using namespace npp;
        using namespace npp::pipe;

        *this | fixed_height(1uL);
        units | bg(color::White) | fg(color::Teal) | fixed_width(2uL);
    }
};

struct Grid_hi_res : npp::layout::Horizontal<npp::HCheckbox_label> {
   public:
    Child_t& grid_box   = this->make_child({L"Grid"});
    Child_t& hi_res_box = this->make_child({L"Hi-Res"});

   public:
    Grid_hi_res()
    {
        using namespace npp::pipe;
        *this | fixed_height(1uL) | children() |
            for_each([](auto& c) { c.padding | fixed_width(1uL); });
        hi_res_box.checkbox.toggle();
    }
};

struct Controls_box : npp::layout::Vertical<> {
   public:
    // Widget& break_                    = this->append_child(make_break());
    Interval_box& interval_edit        = this->make_child<Interval_box>();
    Widget& break_0                    = this->append_child(make_break());
    Start_pause_btns& start_pause_btns = this->make_child<Start_pause_btns>();
    Clear_step_box& clear_step_btns    = this->make_child<Clear_step_box>();
    Widget& break_1                    = this->append_child(make_break());
    Grid_hi_res& grid_hi_res           = this->make_child<Grid_hi_res>();
    npp::HCheckbox_label& rainbow_btn =
        this->make_child<npp::HCheckbox_label>({L"Rainbow Mode"});
    Widget& break_2      = this->append_child(make_break());
    Rule_edit& rule_edit = this->make_child<Rule_edit>();
    Widget& break_3      = this->append_child(make_break());

   public:
    sl::Signal<void(std::string const&)>& rule_change = rule_edit.rule_change;
    sl::Signal<void(std::chrono::milliseconds)> interval_set;
    sl::Signal<void()>& grid_toggled = grid_hi_res.grid_box.checkbox.toggled;
    sl::Signal<void()>& hi_res_toggled =
        grid_hi_res.hi_res_box.checkbox.toggled;
    sl::Signal<void()>& clear_request  = clear_step_btns.clear_btn.pressed;
    sl::Signal<void()>& step_request   = clear_step_btns.step_btn.pressed;
    sl::Signal<void()>& rainbow_toggle = rainbow_btn.checkbox.toggled;

   public:
    Controls_box()
    {
        using namespace npp::pipe;
        *this | fixed_height(12uL);

        interval_edit.value_set.connect([this](int value) {
            interval_set(std::chrono::milliseconds{value});
        });
        interval_edit.value_edit.number_edit | ghost(color::Teal);
    }
};

}  // namespace gol
#endif  // NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_SETTINGS_BOX_HPP
