#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_IS_SENDABLE_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_IS_SENDABLE_HPP
#include "ncursesplusplus/system/event.hpp"

namespace npp {
namespace detail {

/// Defaults to sending if the reciever of the event is enabled.
template<typename E>
auto IsSendable(const E &event) -> bool {
  return event.receiver.get().is_enabled();
}

// Always Sendable
inline auto IsSendable(ChildAddedEvent const &) -> bool { return true; }
inline auto IsSendable(ChildRemovedEvent const &) -> bool { return true; }
inline auto IsSendable(DeleteEvent const &) -> bool { return true; }
inline auto IsSendable(DisableEvent const &) -> bool { return true; }
inline auto IsSendable(FocusOutEvent const &) -> bool { return true; }
inline auto IsSendable(CustomEvent const &) -> bool { return true; }

}
}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_IS_SENDABLE_HPP
