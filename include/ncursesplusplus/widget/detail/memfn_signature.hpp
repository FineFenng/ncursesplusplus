#ifndef NCURSESPLUSPLUS_WIDGET_DETAIL_MEMFN_SIGNATURE_HPP
#define NCURSESPLUSPLUS_WIDGET_DETAIL_MEMFN_SIGNATURE_HPP
namespace npp::detail {

// Could be moved into Signals library with a deduction guide for Slot.

/// Deduce a function's signature type.
template <typename>
struct MemFn_signature {};

template <typename R, typename T, bool N, typename... Args>
struct MemFn_signature<R (T::*)(Args...) noexcept(N)> {
    using type = R(Args...);
};

template <typename R, typename T, bool N, typename... Args>
struct MemFn_signature<R (T::*)(Args...)& noexcept(N)> {
    using type = R(Args...);
};

template <typename R, typename T, bool N, typename... Args>
struct MemFn_signature<R (T::*)(Args...) const noexcept(N)> {
    using type = R(Args...);
};

template <typename R, typename T, bool N, typename... Args>
struct MemFn_signature<R (T::*)(Args...) const& noexcept(N)> {
    using type = R(Args...);
};

/// Deduce operator() type of F
template <typename F>
using Call_signature_t =
    typename MemFn_signature<decltype(&F::operator())>::type;

}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_WIDGET_DETAIL_MEMFN_SIGNATURE_HPP
