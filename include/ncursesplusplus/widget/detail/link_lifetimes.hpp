#ifndef NCURSESPLUSPLUS_WIDGET_DETAIL_LINK_LIFETIMES_HPP
#define NCURSESPLUSPLUS_WIDGET_DETAIL_LINK_LIFETIMES_HPP
#include <utility>

#include <signals_light/signal.hpp"

#include "ncursesplusplus/widget/detail/memfn_signature.hpp"

namespace npp::slot {

/// Return sl::Slot wrapping \p method, tracking the lifetime of all \p widgets.
/** If any of the Widgets are destroyed, the Slot will expire. */
template<typename... Widgets,
    typename F,
    typename Signature = detail::Call_signature_t<F>>
auto link_lifetimes(F &&method, Widgets &... widgets) -> sl::Slot<Signature> {
  static_assert(sizeof...(widgets) > 0, "You must link at least 1 lifetime");
  auto slot = sl::Slot < Signature > {std::forward<F>(method)};
  (slot.track(widgets.lifetime), ...);
  return slot;
}

}  // namespace npp::slot
#endif  // NCURSESPLUSPLUS_WIDGET_DETAIL_LINK_LIFETIMES_HPP
