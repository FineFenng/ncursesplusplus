#ifndef NCURSESPLUSPLUS_COMMON_TRANFORM_VIEW_HPP
#define NCURSESPLUSPLUS_COMMON_TRANFORM_VIEW_HPP
#include <cstddef>
#include <type_traits>
#include <utility>


#include "ncursesplusplus/common/meta.hpp"
#include "ncursesplusplus/common/transform_iterator.hpp"

namespace npp {

/// Read only Container view that applies transformation to elements at access.
/** Container is assumed to have a vector like interface. Transformation is
 *  applied at iterator dereference and operator[]. */
template<typename Container, typename Map_fn>
class TransformView {
 public:
  using Reference =
      typename invoke_result<Map_fn, typename Container::reference>::type;
  using Reference_const =
      typename invoke_result<Map_fn, typename Container::const_reference>::type;

 public:
  TransformView(Container &c, Map_fn map_fn)
      : container_{c}, map_fn_{std::move(map_fn)} {}

 public:
  std::size_t size() const { return container_.size(); }

  bool empty() const { return container_.empty(); }

  Reference_const operator[](std::size_t i) const {
    return map_fn_(container_[i]);
  }

  Reference operator[](std::size_t i) {
    return map_fn_(container_[i]);
  }

  Reference_const front() const {
    return map_fn_(container_.front());
  }

  Reference front() { return map_fn_(container_.front()); }

  Reference_const back() const { return map_fn_(container_.back()); }

  Reference back() { return map_fn_(container_.back()); }

  auto begin() const {
    return TransformIterator{std::cbegin(container_), map_fn_};
  }

  auto begin() { return TransformIterator{std::begin(container_), map_fn_}; }

  auto end() const {
    return TransformIterator{std::cend(container_), map_fn_};
  }

  auto end() { return TransformIterator{std::end(container_), map_fn_}; }

 private:
  Container &container_;
  Map_fn map_fn_;
};

}  // namespace npp
#endif  // NCURSESPLUSPLUS_COMMON_TRANFORM_VIEW_HPP
