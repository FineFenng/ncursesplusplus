#ifndef NCURSESPLUSPLUS_WIDGET_WIDGETS_OPEN_FILE_HPP
#define NCURSESPLUSPLUS_WIDGET_WIDGETS_OPEN_FILE_HPP
#include <fstream>

#include <signals_light/signal.hpp"

#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/widget/layouts/horizontal.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/widgets/button.hpp"
#include "ncursesplusplus/widget/widgets/line_edit.hpp"

namespace npp {

template<typename Char_t = char>
class Open_file : public layout::Horizontal<> {
 public:
  Button &open_btn = this->make_child<Button>("Open");
  Widget &separator = this->make_child() | pipe::fixed_width(1);
  Line_edit &filename_edit = this->make_child<Line_edit>("Filename");

  sl::Signal<void(std::basic_ifstream<Char_t> &)> open_requested;

 public:
  Open_file() {
    this->height_policy.fixed(1);
    open_btn.width_policy.fixed(4);
    open_btn.brush.background = Color::Blue;
    separator.set_wallpaper(L'>');
    filename_edit.brush.background = Color::White;
    filename_edit.brush.foreground = Color::Black;
    filename_edit.set_ghost_color(Color::Dark_gray);
    open_btn.pressed.connect([this] { this->notify(); });
  }

 private:
  void notify() {
    auto ifs = std::basic_ifstream<Char_t>{filename_edit.contents().str()};
    open_requested(ifs);
  }
};

/// Helper function to create an instance.
template<typename Char_t = char>
inline auto open_file() -> std::unique_ptr<Open_file<Char_t>> {
  return std::make_unique<Open_file<Char_t>>();
}

}  // namespace npp
#endif  // NCURSESPLUSPLUS_WIDGET_WIDGETS_OPEN_FILE_HPP
