#include "ncursesplusplus/widget/widget.hpp"

#include <cstdint>
#include <mutex>
#include <string>
#include <utility>

#include <signals_light/signal.hpp"

#include "ncursesplusplus/painter/brush.hpp"
#include "ncursesplusplus/painter/glyph.hpp"
#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/terminal/terminal.hpp"

namespace {

auto get_unique_id() -> std::uint16_t {
  static auto mtx = std::mutex{};
  static auto current = std::uint16_t{0};
  auto const lock = std::scoped_lock{mtx};
  return ++current;
}

void post_child_polished(npp::Widget &w) {
  using namespace npp;
  auto *parent = w.parent();
  if (parent != nullptr)
    System::post_event(ChildPolishedEvent{*parent, w});
}

}  // namespace

namespace npp {

Widget::Widget(std::string name)
    : name_{std::move(name)}, unique_id_{get_unique_id()} {
  width_policy.policy_updated.connect([this] { post_child_polished(*this); });
  height_policy.policy_updated.connect(
      [this] { post_child_polished(*this); });
}

void Widget::enable(bool enable, bool post_child_polished_event) {
  this->enable_and_post_events(enable, post_child_polished_event);
  for (Widget &w : this->get_children())
    w.enable(enable, post_child_polished_event);
}

// Don't want to include system/event.hpp in widget.hpp
void Widget::update() { System::post_event(Paint_event{*this}); }

auto Widget::generate_wallpaper() const -> Glyph {
  auto bg_glyph = wallpaper_ ? *(wallpaper_) : System::terminal.background();
  if (this->does_paint_wallpaper_with_brush())
    bg_glyph.brush = merge(bg_glyph.brush, this->brush);
  return bg_glyph;
}

void Widget::install_event_filter(Widget &filter) {
  if (&filter == this)
    return;
  auto const result = event_filters_.insert(&filter);
  if (result.second) {  // if insert happened
    // Remove filter from list on destruction of filter
    auto remove_on_destroy = sl::Slot < void() > {
        [this, &filter]() { this->remove_event_filter(filter); }};
    // In case *this has been destroyed and the filter has not.
    remove_on_destroy.track(this->lifetime);
    filter.destroyed.connect(remove_on_destroy);
  }
}

void Widget::enable_and_post_events(bool enable, bool post_child_polished_event) {
  if (enabled_ == enable)
    return;
  if (!enable)
    System::post_event(Disable_event{*this});
  enabled_ = enable;
  if (enable)
    System::post_event(Enable_event{*this});
  if (post_child_polished_event and this->parent() != nullptr)
    System::post_event(ChildPolishedEvent{*this->parent(), *this});
  this->update();
}

}  // namespace npp
