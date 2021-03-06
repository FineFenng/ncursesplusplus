#ifndef NCURSESPLUSPLUS_WIDGET_WIDGETS_BUTTON_LIST_HPP
#define NCURSESPLUSPLUS_WIDGET_WIDGETS_BUTTON_LIST_HPP
#include <memory>
#include <string>

#include <signals_light/signal.hpp"

#include "ncursesplusplus/widget/layouts/opposite.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/widget.hpp"
#include "ncursesplusplus/widget/widgets/button.hpp"
#include "ncursesplusplus/widget/widgets/scrollbar.hpp"

namespace npp {

/// A list of buttons with connected scrollbar.
template<template<typename> class Layout_t>
class Button_list : public layout::Opposite_t<Layout_t<Widget>> {
 private:
  using Base_t = layout::Opposite_t<Layout_t<Widget>>;

  static auto constexpr is_vertical = layout::is_vertical_v<Base_t>;

 private:
  class Button_list_impl : public Layout_t<Button> {
   private:
    using Base_t = Layout_t<Button>;

    static auto constexpr is_vertical = layout::is_vertical_v<Base_t>;

   public:
    sl::Signal<void(std::wstring const &name)> button_pressed;

   public:
    auto add_button(std::wstring const &name) -> Button & {
      using namespace npp::pipe;
      auto &btn = this->template MakeChild<Button>(name) |
          on_press([this, name] { button_pressed(name); });
      if constexpr (is_vertical)
        btn | fixed_height(1uL);
      else
        btn | fixed_width(1uL);
      return btn;
    }
  };

 private:
  Scrollbar<Layout_t<Widget>> &scrollbar =
      this->template MakeChild<Scrollbar < Layout_t<Widget>> > ();

  Button_list_impl &btn_list = this->template MakeChild<Button_list_impl>();

 public:
  sl::Signal<void(std::wstring const &name)> &button_pressed =
      btn_list.button_pressed;

 public:
  Button_list() { link(scrollbar, btn_list); }

 public:
  /// Returns reference to the Button added.
  /** The Brush and the 'pressed' signal are accessible to customize. */
  auto add_button(std::wstring const &name) -> Button & {
    return btn_list.add_button(name);
  }

  /// Set the background color of the Scrollbar.
  void set_scrollbar_bg(Color c) { scrollbar.middle.set_bar_bg(c); }

  /// Set the foreground color of the Scrollbar.
  void set_scrollbar_fg(Color c) { scrollbar.middle.set_bar_fg(c); }
};

/// Helper function to create an instance.
template<template<typename> class Layout_t>
auto button_list() -> std::unique_ptr<Button_list<Layout_t>> {
  return std::make_unique<Button_list<Layout_t>>();
}

}  // namespace npp
#endif  // NCURSESPLUSPLUS_WIDGET_WIDGETS_BUTTON_LIST_HPP
