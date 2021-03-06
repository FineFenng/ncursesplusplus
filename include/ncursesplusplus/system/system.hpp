#ifndef NCURSESPLUSPLUS_SYSTEM_SYSTEM_HPP
#define NCURSESPLUSPLUS_SYSTEM_SYSTEM_HPP
#include <mutex>
#include <utility>
#include <vector>

#include <signals_light/signal.hpp"

#include "ncursesplusplus/system/detail/user_input_event_loop.hpp"
#include "ncursesplusplus/system/event_fwd.hpp"
#include "ncursesplusplus/terminal/terminal.hpp"

namespace npp {
class AnimationEngine;
class Widget;
}  // namespace npp

namespace npp::detail {
class Event_engine;
}  // namespace npp::detail

namespace npp {

/// Organizes the highest level of the TUI framework.
/** Constructing an instance of this class initializes the display system.
 *  Manages the head Widget and the main UserInputEventLoop. */
class System {
 public:
  /// Emitted when System::exit is called. Should call EventLoop::exit.
  /** Passes along the exit_code System::exit() was called with. */
  static sl::Signal<void(int)> exit_signal;

  // Slots
  static sl::Slot<void()> quit;

  /// Provides access to and modification of global terminal properties.
  inline static Terminal terminal;

 public:
  System() = default;
  System(System const &) = delete;
  System &operator=(System const &) = delete;
  System(System &&) = default;
  System &operator=(System &&) = default;

  ~System() { System::exit(0); }

  /// Return a pointer to the currently focused Widget.
  static auto focus_widget() -> Widget *;

  /// Give program focus to \p w.
  /** Sends FocusOutEvent to Widget in focus, and FocusInEvent to \p w. */
  static void set_focus(Widget &w);

  /// Removes focus from the currently in focus Widget.
  static void clear_focus();

  /// Enable Tab/Back_tab keys to change the focus Widget.
  static void enable_tab_focus();

  /// Disable Tab/Back_tab keys from changing focus Widget.
  static void disable_tab_focus();

  /// Set a new head Widget for the entire system.
  /** Will disable the previous head widget if not nullptr. Only valid to call
   *  before System::run or after System::exit. */
  static void set_head(Widget *new_head);

  /// Return a pointer to the head Widget.
  /** This Widget is the ancestor of every other widget that will be displayed
   *  on the screen. */
  static auto head() -> Widget * { return head_; }

  /// Create a Widget_t object, set it as head widget and call System::run().
  /** \p args... are passed on to the Widget_t constructor. Blocks until
   *  System::exit() is called, returns the exit code. Will throw a
   *  std::runtime_error if screen cannot be initialized. */
  template<typename Widget_t, typename... Args>
  auto run(Args &&... args) -> int {
    auto head = Widget_t(std::forward<Args>(args)...);
    System::set_head(&head);
    return this->run();
  }

  /// Set \p head as head widget and call System::run().
  /** Will throw a std::runtime_error if screen cannot be initialized. */
  auto run(Widget &head) -> int {
    System::set_head(&head);
    return this->run();
  }

  /// Launch the main EventLoop and start processing Events.
  /** Blocks until System::exit() is called, returns the exit code. Will throw
   *  a std::runtime_error if screen cannot be initialized. Enables and sets
   *  focus to the head Widget.*/
  static auto run() -> int;

  /// Immediately send the event filters and then to the intended receiver.
  static void send_event(Event e);

  // Minor optimization.
  static void send_event(PaintEvent e);

  // Minor optimization.
  static void send_event(DeleteEvent e);

  /// Append the event to the EventQueue for the thread it was called on.
  /** The EventQueue is processed once per iteration of the EventLoop. When
   *  the Event is pulled from the EventQueue, it is processed by
   *  System::send_event() */
  static void post_event(Event e);

  /// Send an exit signal to each of the currently running Event_loops.
  /** Also call Shutdown() on the AnimationEngine and set
   *  System::exit_requested_ to true. Though it sends the exit signal to each
   *  of the Event_loops, they are not guaranteed to be stopped by the time
   *  this function returns. */
  static void exit(int exit_code = 0);

  /// Returns a reference to the single Event_engine that owns the queue.
  static auto event_engine() -> detail::Event_engine & {
    return event_engine_;
  }

  /// Return a reference to the AnimationEngine in System.
  /** This manages animation on each of the Widgets that enables it. */
  static auto animation_engine() -> AnimationEngine & {
    return animation_engine_;
  }

  /// Return whether System has gotten an exit request, set by System::exit()
  static auto exit_requested() -> bool { return exit_requested_; }

 private:
  inline static Widget *head_ = nullptr;
  inline static bool exit_requested_ = false;
  static detail::UserInputEventLoop user_input_loop_;
  static detail::Event_engine event_engine_;
  static AnimationEngine animation_engine_;
};

}  // namespace npp
#endif  // NCURSESPLUSPLUS_SYSTEM_SYSTEM_HPP
