#include "ncursesplusplus/widget/widgets/line_edit.hpp"

#include <cctype>
#include <functional>
#include <utility>
#include <vector>

#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/painter/glyph.hpp"
#include "ncursesplusplus/painter/glyph_string.hpp"
#include "ncursesplusplus/painter/trait.hpp"
#include "ncursesplusplus/system/key.hpp"
#include "ncursesplusplus/widget/point.hpp"
#include "ncursesplusplus/widget/widgets/textbox.hpp"

namespace npp {

void Line_edit::underline(bool enabled) {
  if (enabled) {
    this->set_wallpaper(L' ' | Trait::Underline);
    this->set_contents(this->contents() | Trait::Underline);
    this->insert_brush.add_traits(Trait::Underline);
  } else {
    this->set_wallpaper(L' ');
    auto non_underlined = this->contents();
    non_underlined.remove_traits(Trait::Underline);
    this->set_contents(std::move(non_underlined));
    this->insert_brush.remove_traits(Trait::Underline);
  }
  this->update();
}

void Line_edit::set_ghost_color(Color c) {
  if (not on_initial_)
    return;
  this->set_contents(this->contents() | fg(c));
  this->update();
}

auto Line_edit::key_press_event(Key k) -> bool {
  switch (k) {
    case Key::Enter:edit_finished(this->contents().str());
      if (clear_on_enter_)
        this->clear();
      if (on_initial_)
        on_initial_ = false;
      return true;
    case Key::Arrow_up:
    case Key::Arrow_down: return true;
    default: break;
  }
  auto const is_printable = [](char c) {
    return std::isprint(c) or std::isspace(c);
  };
  auto const symbol = to_wchar(k);
  if (!is_printable(symbol))
    return Textbox::key_press_event(k);
  if (!validator_(symbol))
    return true;
  if (symbol != L'\0' && on_initial_) {  // First alpha-num input
    this->clear();
    on_initial_ = false;
  }
  return Textbox::key_press_event(k);
}

}  // namespace npp
