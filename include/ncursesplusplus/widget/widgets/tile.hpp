#ifndef NCURSESPLUSPLUS_WIDGET_WIDGETS_TILE_HPP
#define NCURSESPLUSPLUS_WIDGET_WIDGETS_TILE_HPP
#include "ncursesplusplus/painter/glyph.hpp"
#include "ncursesplusplus/painter/painter.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/widget.hpp"

namespace npp {

/// A unit width/height Widget that can display a single Glyph.
class Tile : public Widget {
 public:
  using Parameters = Glyph;

 public:
  Tile(Glyph g = L' ') : display_{g} {
    using namespace pipe;
    *this | fixed_width(1) | fixed_height(1);
  }

 public:
  void set(Glyph g) {
    display_ = g;
    this->update();
  }

  auto get() -> Glyph { return display_; }

 protected:
  auto paint_event() -> bool override {
    Painter{*this}.put(display_, {0, 0});
    return Widget::paint_event();
  }

 private:
  Glyph display_;
};

/// Helper function to create an instance.
template<typename... Args>
auto tile(Args &&... args) -> std::unique_ptr<Tile> {
  return std::make_unique<Tile>(std::forward<Args>(args)...);
}

}  // namespace npp
#endif  // NCURSESPLUSPLUS_WIDGET_WIDGETS_TILE_HPP
