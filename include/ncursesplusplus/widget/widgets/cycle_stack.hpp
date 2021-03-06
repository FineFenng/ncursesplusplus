#ifndef NCURSESPLUSPLUS_WIDGET_WIDGETS_CYCLE_STACK_HPP
#define NCURSESPLUSPLUS_WIDGET_WIDGETS_CYCLE_STACK_HPP
#include <memory>
#include <type_traits>
#include <utility>

#include "ncursesplusplus/painter/brush.hpp"
#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/painter/glyph_string.hpp"
#include "ncursesplusplus/painter/trait.hpp"
#include "ncursesplusplus/widget/layouts/horizontal.hpp"
#include "ncursesplusplus/widget/layouts/stack.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/widget.hpp"
#include "ncursesplusplus/widget/widgets/button.hpp"
#include "ncursesplusplus/widget/widgets/cycle_box.hpp"

namespace npp {

/// A layout::Stack with an interface to cycle through each Widget in the stack.
template<typename Child = Widget>
class Cycle_stack : public layout::Vertical<> {
 private:
  /// User interface to cycle through the pages of the Stack.
  class Top_row : public layout::Horizontal<> {
   public:
    Button &left_btn = this->MakeChild<Button>(L"<");
    Cycle_box &cycle_box = this->MakeChild<Cycle_box>();
    Button &right_btn = this->MakeChild<Button>(L">");

   public:
    Top_row() {
      using namespace pipe;
      *this | fixed_height(1) | children() | bg(Color::Light_gray) |
          fg(Color::Black);

      left_btn | fixed_width(1) | on_press(slot::previous(cycle_box));
      right_btn | fixed_width(1) | on_press(slot::next(cycle_box));
      cycle_box | Trait::Bold;
    }
  };

 public:
  Top_row &top_row = this->MakeChild<Top_row>();
  layout::Stack<Child> &stack = this->MakeChild < layout::Stack < Child >> ();

 public:
  /// Construct a new Widget_t object and add it to the end of the Stack.
  /** Returns a reference to this newly created page. \p title is passed to
   *  the Cycle_box to display when this page is active. */
  template<typename Widget_t = Child, typename... Args>
  auto make_page(Glyph_string title, Args &&... args) -> Widget_t & {
    static_assert(std::is_base_of<Child, Widget_t>::value,
                  "Cycle_stack::make_page: Widget_t must be a Child type");
    auto child = std::make_unique<Widget_t>(std::forward<Args>(args)...);
    return static_cast<Widget_t &>(
        this->append_page(std::move(title), std::move(child)));
  }

  /// Append a page to the Stack.
  /** \p title is passed to the Cycle_box associated with this page. */
  auto append_page(Glyph_string title, std::unique_ptr<Child> widget)
  -> Child & {
    auto &signal = top_row.cycle_box.add_option(std::move(title));
    signal.connect(slot::set_active_page(stack, stack.size()));
    auto &child = stack.append_page(std::move(widget));
    if (stack.size() == 1)
      stack.set_active_page(0);
    return child;
  }
};

/// Helper function to create an instance.
template<typename Child = Widget, typename... Args>
auto cycle_stack(Args &&... args) -> std::unique_ptr<Cycle_stack<Child>> {
  return std::make_unique<Cycle_stack<Child>>(std::forward<Args>(args)...);
}

}  // namespace npp
#endif  // NCURSESPLUSPLUS_WIDGET_WIDGETS_CYCLE_STACK_HPP
