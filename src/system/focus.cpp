#include "ncursesplusplus/system/detail/focus.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/widget/focus_policy.hpp"
#include "ncursesplusplus/widget/widget.hpp"

namespace {
using namespace npp;
using namespace npp::detail;

auto is_tab_focus_policy(Focus_policy policy) -> bool {
  return policy == Focus_policy::Strong || policy == Focus_policy::Tab;
}

auto is_click_focus_policy(Focus_policy policy) -> bool {
  return policy == Focus_policy::Strong || policy == Focus_policy::Click;
}

auto const is_tab_focusable = [](auto const *widg) {
  return widg->is_enabled() && is_tab_focus_policy(widg->focus_policy);
};

// Return a widg tree from System::head() if FocusWidget is nullptr.
auto gen_focus_front_widg_tree() -> std::vector<npp::Widget *> {
  auto widg_tree = System::head()->get_descendants();
  widg_tree.insert(std::begin(widg_tree), System::head());
  auto *const focus_widg = Focus::FocusWidget();
  if (focus_widg != nullptr) {
    auto const begin = std::begin(widg_tree);
    auto const end = std::end(widg_tree);
    auto const iter = std::find(begin, end, focus_widg);
    if (iter != end)
      std::rotate(begin, iter, end);
  }
  return widg_tree;
}

auto next_tab_focus() -> npp::Widget * {
  if (System::head() == nullptr)
    return nullptr;
  auto const widg_tree = gen_focus_front_widg_tree();
  auto const begin = std::next(std::begin(widg_tree));
  auto const end = std::end(widg_tree);
  auto const next_focus = std::find_if(begin, end, is_tab_focusable);
  return (next_focus != end) ? *next_focus : Focus::FocusWidget();
}

auto previous_tab_focus() -> npp::Widget * {
  if (System::head() == nullptr)
    return nullptr;
  auto const widg_tree = gen_focus_front_widg_tree();
  auto const begin = std::rbegin(widg_tree);
  auto const end = std::rend(widg_tree);
  auto const previous_focus = std::find_if(begin, end, is_tab_focusable);
  return previous_focus != end ? *previous_focus : Focus::FocusWidget();
}

}  // namespace

namespace npp::detail {

npp::Widget *Focus::focus_widget_ = nullptr;
bool Focus::tab_enabled_ = true;
bool Focus::tab_suppressed_ = false;

void Focus::MousePress(npp::Widget &clicked) {
  if (&clicked == focus_widget_)
    return;
  if (is_click_focus_policy(clicked.focus_policy))
    Focus::Set(clicked);
}

auto Focus::TabPress() -> bool {
  if (tab_enabled_ && !tab_suppressed_) {
    auto *next = next_tab_focus();
    if (next == nullptr)
      Focus::Clear();
    else
      Focus::Set(*next);
    return true;
  }
  tab_suppressed_ = false;
  return false;
}

auto Focus::ShiftTabPress() -> bool {
  if (tab_enabled_ && !tab_suppressed_) {
    auto *previous = previous_tab_focus();
    if (previous == nullptr)
      Focus::Clear();
    else
      Focus::Set(*previous);
    return true;
  }
  tab_suppressed_ = false;
  return false;
}

void Focus::Set(npp::Widget &new_focus) {
  if (&new_focus == focus_widget_)
    return;
  if (new_focus.focus_policy == Focus_policy::None) {
    Focus::Clear();
    return;
  }
  if (focus_widget_ != nullptr)
    System::send_event(Focus_out_event{*focus_widget_});
  System::send_event(Focus_in_event{new_focus});
}

void Focus::Clear() {
  if (focus_widget_ == nullptr)
    return;
  System::post_event(Focus_out_event{*focus_widget_});
  focus_widget_ = nullptr;
}

}  // namespace npp::detail
