#ifndef NCURSESPLUSPLUS_COMMON_LOCKABLE_HPP
#define NCURSESPLUSPLUS_COMMON_LOCKABLE_HPP
#include <mutex>

namespace npp {

template <typename Mutex = std::mutex>
class Lockable {
   public:
    auto lock() -> std::scoped_lock<Mutex> { return std::scoped_lock{mtx_}; }

    auto mutex() -> Mutex& { return mtx_; }

   private:
    Mutex mtx_;
};

}  // namespace npp
#endif  // NCURSESPLUSPLUS_COMMON_LOCKABLE_HPP
