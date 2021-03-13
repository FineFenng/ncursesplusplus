#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_FOCUS_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_FOCUS_HPP

namespace npp {

class Widget;
struct FocusInEvent;

}  // namespace npp

namespace npp {
namespace detail {

class Focus {
 public:
  /// Return a pointer to the currently focused Widget, can return nullptr.
  static Widget* FocusWidget() { return focus_widget_; }

  /// Sets the focus to \p clicked if it has a Focus_policy::Mouse/Strong.
  static void MousePress(npp::Widget &clicked);

  /// Find the next Focus_policy::Tab/Strong Widget and set focus to it.
  static bool TabPress();

  /// Find the previous Focus_policy::Tab/Strong Widget and set focus to it.
  static bool ShiftTabPress();

  /// Sets focus to \p new_focus, via a posted Focus_in event.
  /** If \p new_focus has Focus_policy::None, calls Focus::clear(). */
  static void Set(npp::Widget &new_focus);

  /// Set the focus widget to nullptr.
  static void Clear();

  /// Enable Tab/Back_tab keys to change the focus Widget.
  static void EnableTabFocus() { tab_enabled_ = true; }

  /// Disable Tab/Back_tab keys from changing focus Widget.
  static void DisableTabFocus() { tab_enabled_ = false; }

  /// Stops a Tab or Back_tab from changing focus to the next Widget.
  static void SuppressTab() { tab_suppressed_ = true; }

  /// Re-enable a Tab or Back_tab to change focus to the next Widget.
  static void UnsuppressTab() { tab_suppressed_ = false; }

  /// Directly sets the focus widget without sending any events.
  /** Only for use by send(FocusInEvent). */
  static void DirectSetFocus(Widget* widget) { focus_widget_ = widget; }

 private:
  static Widget *focus_widget_;
  static bool tab_enabled_;
  static bool tab_suppressed_;
};

}
}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_FOCUS_HPP
