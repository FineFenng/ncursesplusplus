#ifndef NCURSESPLUSPLUS_COMMON_IDENTITY_HPP
#define NCURSESPLUSPLUS_COMMON_IDENTITY_HPP
#include <utility>

#include ""

namespace ox {

/// Provides an Identity projection operator(), overloaded for any type.
struct Identity {
  template<typename T>
  constexpr auto operator()(T &&x) const -> T && {
    return std::forward<T>(x);
  }
};

}  // namespace ox
#endif  // NCURSESPLUSPLUS_COMMON_IDENTITY_HPP
