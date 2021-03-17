#ifndef NCURSESPLUSPLUS_COMMON_TRANSFORM_ITERATOR_HPP
#define NCURSESPLUSPLUS_COMMON_TRANSFORM_ITERATOR_HPP
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

#include "ncursesplusplus/common/meta.hpp"

namespace npp {

/// operator*() will apply \p map to the result of the underlying deref result.
/** Should work both as a const_iterator and non-const iterator, depending on
 *  the type of iterator given at construction, and if the map_fn works in both
 *  const and non-const conditions. */
template<typename Iter, typename Map_fn>
class TransformIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using reference = typename invoke_result<Map_fn, typename Iter::reference>::type;
  using value_type = typename std::remove_reference<reference>::type;
  using pointer = value_type *;

 public:
  TransformIterator(Iter it, Map_fn map_fn)
      : it_{it}, map_fn_{std::move(map_fn)} {}

  TransformIterator(TransformIterator const &) = default;
  TransformIterator(TransformIterator &&)  noexcept = default;

  TransformIterator &operator=(TransformIterator const &other) {
    it_ = other.it_;
    return *this;
  }
  TransformIterator &operator=(TransformIterator &&)  noexcept = default;

 public:
  auto operator++() -> TransformIterator & {
    ++it_;
    return *this;
  }

  auto operator++(int) -> TransformIterator { return {it_++, map_fn_}; }

  auto operator*() const -> reference { return map_fn_(*it_); }

  auto operator==(TransformIterator const &other) const -> bool {
    return it_ == other.it_;
  }

  auto operator!=(TransformIterator const &other) const -> bool {
    return it_ != other.it_;
  }

  template<typename T>
  auto operator==(T const &other) const -> bool {
    return it_ == other;
  }

  template<typename T>
  auto operator!=(T const &other) const -> bool {
    return it_ != other;
  }

  auto underlying() const -> Iter { return it_; }

 private:
  Iter it_;
  Map_fn map_fn_;
};

}  // namespace npp
#endif  // NCURSESPLUSPLUS_COMMON_TRANSFORM_ITERATOR_HPP
