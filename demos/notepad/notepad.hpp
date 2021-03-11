#ifndef DEMOS_NOTEPAD_NOTEPAD_HPP
#define DEMOS_NOTEPAD_NOTEPAD_HPP
#include <chrono>
#include <cmath>

#include <signals_light/signal.hpp>

#include <termox/painter/glyph_string.hpp>
#include <termox/painter/trait.hpp>
#include <termox/system/system.hpp>
#include <termox/widget/align.hpp>
#include <termox/widget/focus_policy.hpp>
#include <termox/widget/layouts/horizontal.hpp>
#include <termox/widget/layouts/vertical.hpp>
#include <termox/widget/pipe.hpp>
#include <termox/widget/widget_slots.hpp>
#include <termox/widget/widgets/accordion.hpp>
#include <termox/widget/widgets/banner.hpp>
#include <termox/widget/widgets/button.hpp>
#include <termox/widget/widgets/checkbox.hpp>
#include <termox/widget/widgets/color_select.hpp>
#include <termox/widget/widgets/label.hpp>
#include <termox/widget/widgets/textbox.hpp>

namespace demos {

/// Checkbox to enable/disable a Trait. Has a Signal from it.
class Trait_checkbox : public npp::HCheckbox_label {
   public:
    sl::Signal<void(npp::Trait)> trait_enabled;
    sl::Signal<void(npp::Trait)> trait_disabled;

   public:
    Trait_checkbox(npp::Trait t) : npp::HCheckbox_label{{to_string(t)}}, t_{t}
    {
        this->padding | npp::pipe::fixed_width(2);
        this->checkbox.checked.connect([this] { this->trait_enabled(t_); });
        this->checkbox.unchecked.connect([this] { this->trait_disabled(t_); });
    }

   private:
    npp::Trait const t_;
};

/// Holds a Trait_checkbox for each Trait, emits Signals.
class Trait_boxes : public npp::layout::Vertical<Trait_checkbox> {
   public:
    sl::Signal<void(npp::Trait)> trait_enabled;
    sl::Signal<void(npp::Trait)> trait_disabled;

   public:
    Trait_boxes()
    {
        for (auto i = 0; i < npp::Trait_count; ++i) {
            auto& w = this->make_child(static_cast<npp::Trait>(i));
            w.trait_enabled.connect([this](auto t) { this->trait_enabled(t); });
            w.trait_disabled.connect(
                [this](npp::Trait t) { this->trait_disabled(t); });
        }
    }
};

class Labeled_color_select : public npp::HLabel_top<npp::Color_select> {
   private:
    using Label_t = npp::HLabel_top<npp::Color_select>;

   public:
    sl::Signal<void(npp::Color)>& color_selected = wrapped.color_selected;

   public:
    Labeled_color_select(npp::Glyph_string label) : Label_t{std::move(label)}
    {
        using namespace npp;
        using namespace npp::pipe;

        System::terminal.palette_changed.connect(
            [this](auto const& pal) { this->set_heights(pal); });

        this->set_heights(System::terminal.current_palette());
        this->label | Trait::Bold | align_center();
    }

   private:
    /// Sets Widget heights based on number of colors in palette.
    void set_heights(npp::Palette const& pal)
    {
        using namespace npp::pipe;
        auto const height = std::ceil(pal.size() / 8.);
        *this | fixed_height(height + 1);
        this->wrapped | fixed_height(height);
    }
};

class Side_pane : public npp::layout::Vertical<> {
   public:
    Labeled_color_select& fg_select =
        this->make_child<Labeled_color_select>("Foreground⤵");

    Labeled_color_select& bg_select =
        this->make_child<Labeled_color_select>("Background⤵");

    Trait_boxes& trait_boxes = this->make_child<Trait_boxes>();

   public:
    Side_pane() { *this | npp::pipe::fixed_width(16); }
};

using Side_pane_accordion = npp::HAccordion<Side_pane, npp::Bar_position::Last>;

class Text_and_side_pane : public npp::layout::Horizontal<> {
   public:
    npp::Textbox& textbox = this->make_child<npp::Textbox>();
    Side_pane& side_pane =
        this->make_child<Side_pane_accordion>({L"Settings", npp::Align::Center})
            .wrapped();

   public:
    Text_and_side_pane()
    {
        using namespace npp;
        using namespace npp::pipe;

        textbox | bordered() | rounded_corners() | bg(Color::Dark_gray);

        side_pane.fg_select.color_selected.connect(
                npp::slot::set_foreground(textbox));
        side_pane.bg_select.color_selected.connect(
                npp::slot::set_background(textbox));

        side_pane.trait_boxes.trait_enabled.connect(
            [this](Trait t) { textbox.insert_brush.add_traits(t); });
        side_pane.trait_boxes.trait_disabled.connect(
            [this](Trait t) { textbox.insert_brush.remove_traits(t); });
    }
};

class Filename_edit : public npp::Textbox {
   public:
    Filename_edit()
    {
        using namespace npp;
        using namespace npp::pipe;
        *this | bg(Color::White) | fg(Color::Black);
        this->disable_scrollwheel();
    }

   protected:
    auto key_press_event(npp::Key k) -> bool override
    {
        if (k == npp::Key::Enter)
            return true;
        return Textbox::key_press_event(k);
    }
};

class Save_area : public npp::layout::Horizontal<> {
   public:
    sl::Signal<void(std::string)> save_request;
    sl::Signal<void(std::string)> load_request;

   public:
    Save_area()
    {
        using namespace npp;
        using namespace npp::pipe;

        *this | fixed_height(1);

        load_btn | fixed_width(6) | bg(Color::Blue) | on_press([this] {
            this->load_request(filename_edit.contents().str());
        });

        save_btn | fixed_width(6) | bg(Color::Blue) | on_press([this] {
            this->save_request(filename_edit.contents().str());
        });
    }

   private:
    npp::Button& load_btn         = this->make_child<npp::Button>("Load");
    Filename_edit& filename_edit = this->make_child<Filename_edit>();
    npp::Button& save_btn         = this->make_child<npp::Button>("Save");
};

class File_status_bar : public npp::Banner<npp::animator::Unscramble> {
   public:
    File_status_bar() : Banner{std::chrono::milliseconds{50}} {}

   public:
    void fail(npp::Glyph_string message)
    {
        using namespace npp;
        this->set_text(std::move(message | fg(Color::Red)));
    }

    void success(npp::Glyph_string message)
    {
        using namespace npp;
        this->set_text(std::move(message | fg(Color::Light_green)));
    }
};

class Notepad : public npp::layout::Vertical<> {
   public:
    Text_and_side_pane& txt_trait = this->make_child<Text_and_side_pane>();
    File_status_bar& status_bar   = this->make_child<File_status_bar>();
    Save_area& save_area          = this->make_child<Save_area>();

   public:
    Notepad()
    {
        this->initialize();
        this->focus_policy = npp::Focus_policy::Strong;
    }

   protected:
    auto focus_in_event() -> bool override
    {
        npp::System::set_focus(txt_trait.textbox);
        return true;
    }

   private:
    void initialize();
};

}  // namespace demos
#endif  // DEMOS_NOTEPAD_NOTEPAD_HPP
