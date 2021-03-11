#ifndef TERMOX_DEMOS_GLYPH_PAINT_GLYPH_SELECTOR_HPP
#define TERMOX_DEMOS_GLYPH_PAINT_GLYPH_SELECTOR_HPP
#include <memory>
#include <utility>

#include <signals_light/signal.hpp>

#include <termox/painter/color.hpp>
#include <termox/painter/glyph.hpp>
#include <termox/painter/glyph_string.hpp>
#include <termox/widget/layouts/horizontal.hpp>
#include <termox/widget/layouts/stack.hpp>
#include <termox/widget/layouts/vertical.hpp>
#include <termox/widget/widgets/cycle_stack.hpp>
#include <termox/widget/widgets/scrollbar.hpp>
#include <termox/widget/widgets/textbox.hpp>

namespace paint {

class Glyph_sheet : public npp::Textbox {
   public:
    /// Emitted when a Glyph is clicked on; sends the Glyph.
    sl::Signal<void(npp::Glyph)> selected;

   public:
    Glyph_sheet(npp::Glyph_string symbols) : Textbox{std::move(symbols)}
    {
        this->disable_input();
        this->disable_word_wrap();
        this->cursor.disable();
    }

   protected:
    auto mouse_press_event(npp::Mouse const& m) -> bool override
    {
        auto const& contents = this->contents();
        if (m.button == npp::Mouse::Button::Left && !contents.empty())
            selected(contents[this->index_at(m.local)]);
        return Textbox::mouse_press_event(m);
    }
};

/// A vertically scrollable display of Glyphs.
class Glyph_sheet_scrollable : public npp::layout::Horizontal<> {
   public:
    Glyph_sheet& sheet;
    npp::VScrollbar& scrollbar = make_child<npp::VScrollbar>();

   public:
    /// Emitted when a Glyph is clicked on; sends the Glyph.
    sl::Signal<void(npp::Glyph)>& selected = sheet.selected;

   public:
    Glyph_sheet_scrollable(npp::Glyph_string symbols)
        : sheet{make_child<Glyph_sheet>(std::move(symbols))}
    {
        link(scrollbar, sheet);
    }
};

/// Pages of Glyph_sheets that allows you to select a Glyph by clicking on it.
class Glyph_selector : public npp::Cycle_stack<Glyph_sheet_scrollable> {
   public:
    /// Emitted when a Glyph is clicked on; sends the Glyph.
    sl::Signal<void(npp::Glyph)> selected;

   public:
    /// Creates new page in stack, associated with title and hooks up signals.
    auto add_sheet(npp::Glyph_string title, npp::Glyph_string symbols)
        -> Glyph_sheet&
    {
        auto& result =
            this->Cycle_stack::make_page(std::move(title), std::move(symbols));
        result.selected.connect([this](npp::Glyph g) { this->selected(g); });
        return result.sheet;
    }
};

inline auto glyph_selector() -> std::unique_ptr<Glyph_selector>
{
    return std::make_unique<Glyph_selector>();
}

auto populated_glyph_selector() -> std::unique_ptr<Glyph_selector>;

}  // namespace paint
#endif  // TERMOX_DEMOS_GLYPH_PAINT_GLYPH_SELECTOR_HPP
