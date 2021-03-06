#include "ncursesplusplus/system/system.hpp"

#include <algorithm>
#include <iterator>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#include <signals_light/signal.hpp"

#include "ncursesplusplus/system/animation_engine.hpp"
#include "ncursesplusplus/system/detail/event_engine.hpp"
#include "ncursesplusplus/system/detail/event_queue.hpp"
#include "ncursesplusplus/system/detail/filter_send.hpp"
#include "ncursesplusplus/system/detail/focus.hpp"
#include "ncursesplusplus/system/detail/is_sendable.hpp"
#include "ncursesplusplus/system/detail/send.hpp"
#include "ncursesplusplus/system/detail/user_input_event_loop.hpp"
#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/system/event_loop.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/terminal/terminal.hpp"
#include "ncursesplusplus/widget/area.hpp"
#include "ncursesplusplus/widget/widget.hpp"

namespace npp {

auto System::focus_widget() -> Widget * { return detail::Focus::FocusWidget(); }

void System::set_focus(Widget &w) { detail::Focus::Set(w); }

void System::clear_focus() { detail::Focus::Clear(); }

void System::enable_tab_focus() { detail::Focus::EnableTabFocus(); }

void System::disable_tab_focus() { detail::Focus::DisableTabFocus(); }

void System::post_event(Event e) {
  System::event_engine().queue().Append(std::move(e));
}

void System::exit(int exit_code) {
  System::exit_requested_ = true;
  System::exit_signal(exit_code);
}

void System::set_head(Widget *new_head) {
  if (head_ != nullptr)
    head_->disable();
  head_ = new_head;
}

auto System::run() -> int {
  if (head_ == nullptr)
    return -1;
  terminal.initialize();
  head_->enable();
  System::post_event(ResizeEvent{*System::head(), terminal.area()});
  detail::Focus::Set(*head_);
  auto const exit_code = user_input_loop_.run();
  terminal.uninitialize();
  return exit_code;
}

void System::send_event(Event e) {
  if (!std::visit([](auto const &e) { return detail::IsSendable(e); }, e))
    return;
  auto const handled =
      std::visit([](auto const &e) { return detail::filter_send(e); }, e);
  if (!handled)
    std::visit([](auto e) { detail::Send(std::move(e)); }, std::move(e));
}

void System::send_event(PaintEvent e) {
  if (!detail::IsSendable(e))
    return;
  auto const handled = detail::filter_send(e);
  if (!handled)
    detail::Send(std::move(e));
}

void System::send_event(DeleteEvent e) {
  auto const handled = detail::filter_send(e);
  if (!handled)
    detail::Send(std::move(e));
}

sl::Slot<void()> System::quit = [] { System::exit(); };
detail::Event_engine System::event_engine_;
AnimationEngine System::animation_engine_;

// GCC requires this here, it can't find the default constructor when it's in
// system.hpp for whatever reason...
sl::Signal<void(int)> System::exit_signal;
detail::UserInputEventLoop System::user_input_loop_;

}  // namespace npp
