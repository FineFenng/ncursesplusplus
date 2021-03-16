#ifndef NCURSESPLUSPLUS_COMMON_IS_INVOCABLE_HPP_
#define NCURSESPLUSPLUS_COMMON_IS_INVOCABLE_HPP_

#include <function>
#include <type_traits>

namespace npp {
template<typename F, typename  ... Args>
struct is_invocable :
    std::is_constructible<std::function<void(Args...)>,
    std::reference_wrapper<typename std::remove_reference<F>::type>> {
};


}

#endif //NCURSESPLUSPLUS_COMMON_IS_INVOCABLE_HPP_
