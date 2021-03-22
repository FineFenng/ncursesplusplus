#ifndef NCURSESPLUSPLUS_WIDGET_LAYOUT_HPP
#define NCURSESPLUSPLUS_WIDGET_LAYOUT_HPP
#include <cstddef>

#include <algorithm>
#include <memory>
#include <type_traits>
#include <utility>

#include "ncursesplusplus/common/meta.hpp"
#include "ncursesplusplus/common/transform_view.hpp"
#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/widget/widget.hpp"

#include "absl/meta/type_traits.h"

namespace npp {
struct Area;
struct Point;
}  // namespace npp

namespace npp {
namespace layout {

/// Provided as a uniform interface for arranging child Widgets.
/** Layout is limited to holding Child objects, which much be Widget or some
 *  derived type. */
template<typename Child = Widget>
class Layout : public Widget {
 public:
  static_assert(std::is_base_of<Widget, Child>::value,
                "Layout: Child type must be a Widget type.");

  template<typename UnaryPredicate>
  using EnableIfInvocable = typename std::enable_if<is_invocable<UnaryPredicate,
                                                                 typename std::add_lvalue_reference<Child>::type>::value,
                                                    int>::type;
  DEFINE_HAS_TYPE(Parameters);
  template<typename W>
  using EnableIfHasfParamters = typename std::enable_if<HasTypeOfParameters<W>::value>::type;

 public:
  template<typename Widget>
  explicit Layout(std::unique_ptr<Widget> child) {
    this->AppendChild(std::move(child));
  }

  template<typename Widget, typename... Widgets>
  explicit Layout(std::unique_ptr<Widget> child, std::unique_ptr<Widgets>... children) {
    this->AppendChild(std::move(child));
    Layout(children...);
  }


 public:
  /// Return a View of all children.
  Child& get_children() override {
    static constexpr downcast = [](auto &widg_ptr) -> Child & {
      return static_cast<Child &>(*widg_ptr);
    };
    return TransformView(children_, downcast);
  }

  /// Return a const View of all children.
  auto get_children() const {
    auto constexpr downcast = [](auto const &widg_ptr) -> Child const & {
      return static_cast<Child const &>(*widg_ptr);
    };
    return TransformView(children_, downcast);
  }

  /// Inserts \p w at \p index, sending child added event to *this.
  /** Widget_t must be a Child or derived object. Inserts at end of
   *  container if \p index is out of range.  Returns a reference to the
   *  inserted Widget. No checks for nullptr. */
  template<typename W>
  W& InsertChild(std::unique_ptr<W> w, std::size_t index)  {
    static_assert(std::is_base_of<Child, Widget_t>::value,
                  "Layout::insert: Widget_t must be a Child type");
    if (index > this->child_count())
      index = this->child_count();
    auto &inserted = *w;
    children_.emplace(this->iter_at(index), std::move(w));
    inserted.set_parent(this);
    inserted.enable(this->is_enabled());
    System::post_event(ChildAddedEvent{*this, inserted});
    return inserted;
  }

  /// Move \p w to the end of the child container. Forwards to InsertChild()
  /** Returns a reference to the inserted Widget. */
  template<typename W>
  W& AppendChild(std::unique_ptr<W> w) {
    return this->InsertChild(std::move(w), this->child_count());
  }

  /// Create a Widget and append it to the child container.
  /** Return a reference to this newly created Widget. */
  template<typename W = C, typename... Args>
  W& MakeChild(Args &&... args) {
    return this->AppendChild(std::make_unique<W>(std::forward<Args>(args)...));
  }

  // TODO Remove this
  /// Helper so Parameters type does not have to be specified at call site.
  template<typename W = C, typename = EnableIfHasfParamters<C>>
  W& MakeChild(typename C::Parameters p) {
    return this->AppendChild(std::make_unique<W>(std::move(p)));
  }

  /// Removes and returns the child pointed to by \p child.
  /** Returns nullptr if \p child isn't a child of *this. */
  std::unique_ptr<Widget> RemoveChild(const Child *child) {
    const auto& pos = this->find_iter(child);
    if (pos == std::end(children_)) {
      return nullptr;
    }
    auto removed = this->iter_remove(pos);
    this->uninitialize(*removed);
    return removed;
  }

  /// Removes and returns the first child where predicate(child) returns true.
  /** If no child is found, returns nullptr. */
  template<typename UnaryPredicate, typename = EnableIfInvocable<UnaryPredicate>>
  std::unique_ptr<Widget> RemoveChildIf(UnaryPredicate &&predicate) {
    Widget *found = this->FindChildIf(std::forward<UnaryPredicate>(predicate));
    return this->RemoveChild(found);
  }

  /// Removes and returns the child at \p index in the child container.
  /** Returns nullptr if \p index is out of range. */
  std::unique_ptr<Widget> RemoveChildIf(std::size_t index) {
    if (index >= this->child_count())
      return nullptr;
    auto removed = this->iter_remove(this->iter_at(index));
    this->uninitialize(*removed);
    return removed;
  }

  /// Removes the child with given pointer and sends a DeleteEvent to it.
  /** Returns false if \p child is not found and deleted. */
  bool RemoveAndDeleteChild(Child const *child) {
    auto removed = this->RemoveChild(child);
    if (removed == nullptr) {
      return false;
    }
    System::post_event(DeleteEvent{std::move(removed)});
    return true;
  }

  /// Erase first element that satisfies \p pred.
  virtual /** Returns true if a child is found and deleted. */
  template<typename UnaryPredicate, typename = EnableIfInvocable<UnaryPredicate>>
  bool RemoveAndDeleteChildIf(UnaryPredicate &&predicate) {
    Widget *found =
        this->FindChildIf(std::forward<UnaryPredicate>(predicate));
    if (found == nullptr)
      return false;
    this->RemoveAndDeleteChild(found);
    return true;
  }

  /// Removes the child at \p index and sends a DeleteEvent to it.
  /** Returns false if \p index is out of range. */
  auto remove_and_delete_child_at(std::size_t index) -> bool {
    auto removed = this->RemoveChildIf(index);
    if (removed == nullptr)
      return false;
    System::post_event(DeleteEvent{std::move(removed)});
    return true;
  }

  /// Removes all children and sends Delete_events to each.
  void delete_all_children() {
    while (this->child_count() != 0uL)
      this->remove_and_delete_child_at(0uL);
  }

  /// Swap two child widgets, no index range check.
  void swap_children(std::size_t index_a, std::size_t index_b) {
    std::iter_swap(this->iter_at(index_a), this->iter_at(index_b));
    System::post_event(ChildPolishedEvent{*this, *children_[index_b]});
    System::post_event(ChildPolishedEvent{*this, *children_[index_a]});
  }

  /// Find first child satisfying \p predicate.
  /** \p predicate takes a Widget_t const reference and returns a bool.
   *  Returns nullptr if no child is found. */
  template<typename UnaryPredicate>
  const Child* FindChildIf(UnaryPredicate &&predicate) const {
    auto const view = this->get_children();
    auto const found =
        std::find_if(std::cbegin(view), std::cend(view),
                     std::forward<UnaryPredicate>(predicate));
    return found == std::cend(view) ? nullptr : std::addressof(*found);
  }

  /// Find first child satisfying \p predicate.
  /** \p predicate takes a Widget_t const reference and returns a bool.
   *  Returns nullptr if no child is found. */
  template<typename UnaryPredicate>
  Child* FindChildIf(UnaryPredicate &&predicate) {
    auto view = this->get_children();
    const auto& pos =
        std::find_if(view.begin(), view.end(),
                     std::forward<UnaryPredicate>(predicate));
    return pos == view.end() ? nullptr : std::addressof(*pos);
  }

  /// Find a child widget by name, returns nullptr if not found.
  auto find_child_by_name(std::string const &name) -> Child * {
    return this->FindChildIf(
        [&](Child const &w) { return w.name() == name; });
  }

  /// Find a child widget by name, returns nullptr if not found.
  auto find_child_by_name(std::string const &name) const -> Child const * {
    return this->FindChildIf(
        [&](Child const &w) { return w.name() == name; });
  }

  /// Finds the index of the given child pointer in the child container.
  /** Returns std::size_t(-1) if \p w is not a child of *this. */
  auto find_child_position(Child const *w) const -> std::size_t {
    auto const found = std::find_if(
        std::cbegin(children_), std::cend(children_),
        [w](std::unique_ptr<Widget> const &x) { return x.get() == w; });
    if (found == std::cend(children_))
      return -1uL;
    return std::distance(std::cbegin(children_), found);
  }

  /// Returns true if \p w is a child of *this.
  auto contains_child(Child const *w) const -> bool {
    return this->find_child(w) != nullptr;
  }

  /// Returns true if \p descendant is a child or some other child's child etc
  /** Not as efficient as it could be. */
  auto contains_descendant(Widget const *descendant) const -> bool {
    auto const d = this->get_descendants();
    return std::find(std::begin(d), std::end(d), descendant) != std::end(d);
  }

 protected:
  /// Clients override this to post Resize and Move events to children.
  /** This will be called each time the children Widgets possibly need to be
   *  rearranged. Triggered by MoveEvent, ResizeEvent, ChildAddedEvent,
   *  ChildRemovedEvent, ChildPolishedEvent, and Enable_even\. */
  virtual void update_geometry() = 0;

  auto enable_event() -> bool override {
    this->update_geometry();
    return Widget::enable_event();
  }

  auto move_event(Point new_position, Point old_position) -> bool override {
    this->update_geometry();
    return Widget::move_event(new_position, old_position);
  }

  auto resize_event(Area new_size, Area old_size) -> bool override {
    this->update_geometry();
    return Widget::resize_event(new_size, old_size);
  }

  auto child_added_event(Widget &child) -> bool override {
    // ChildAddedEvent can be sent even if receivier is disabled, and
    // update_geometry() is capable of enabling child widgets, so don't call
    if (this->is_enabled())
      this->update_geometry();
    return Widget::child_added_event(child);
  }

  auto child_removed_event(Widget &child) -> bool override {
    // ChildRemovedEvent can be sent even if receivier is disabled, and
    // update_geometry() is capable of enabling child widgets, so don't call
    if (this->is_enabled())
      this->update_geometry();
    return Widget::child_removed_event(child);
  }

  auto child_polished_event(Widget &child) -> bool override {
    this->update_geometry();
    return Widget::child_polished_event(child);
  }

 protected:
  struct Dimensions {
    Widget *widget;
    std::size_t width;
    std::size_t height;
  };

  struct Dimensions_reference {
    Widget *widget;
    std::size_t *width;
    std::size_t *height;
  };

 private:
  /// Get the iterator pointing to the child at \p index into children_.
  auto iter_at(std::size_t index) -> Children_t::iterator {
    return std::next(std::begin(children_), index);
  }

  /// Get the iterator pointing to the child at \p index into children_.
  auto iter_at(std::size_t index) const -> Children_t::const_iterator {
    return std::next(std::cbegin(children_), index);
  }

  /// find_iter implementation.
  auto is_target(Child const *target) {
    return [target](std::unique_ptr<Widget> const &w) -> bool {
      return w.get() == target;
    };
  }

  /// Find the iterator pointing to \p w.
  auto find_iter(Child const *w) const -> Children_t::const_iterator {
    return std::find_if(std::cbegin(children_), std::cend(children_),
                        is_target(w));
  }

  /// Find the iterator pointing to \p w.
  auto find_iter(Child const *w) -> Children_t::iterator {
    return std::find_if(std::begin(children_), std::end(children_),
                        is_target(w));
  }

  /// Moves \p at out of the widget tree and erases the nullptr left behind.
  auto iter_remove(Children_t::iterator at) -> std::unique_ptr<Widget> {
    auto removed = std::move(*at);
    children_.erase(at);
    return removed;
  }

  /// Disable, post Child_remove_event to *this, and set parent to nullptr.
  void uninitialize(Widget &w) {
    w.disable();
    System::post_event(ChildRemovedEvent{*this, w});
    w.set_parent(nullptr);
  }
};

}
}  // namespace npp::layout
#endif  // NCURSESPLUSPLUS_WIDGET_LAYOUT_HPP
