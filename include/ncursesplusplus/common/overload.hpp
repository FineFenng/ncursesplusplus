#ifndef NCURSESPLUSPLUS_COMMON_OVERLOAD_HPP
#define NCURSESPLUSPLUS_COMMON_OVERLOAD_HPP
namespace npp {

/// Overload pattern for function objects.
/** Construct with multiple function objects to get a single object with
 *  overloaded operator() from each function object given. */
template<typename... Ts>
struct Overload : Ts ... {
  using Ts::operator()...;
};

template<typename... Ts>
Overload(Ts...) -> Overload<Ts...>;

}  // namespace npp
#endif  // NCURSESPLUSPLUS_COMMON_OVERLOAD_HPP
