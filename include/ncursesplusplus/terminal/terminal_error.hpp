#ifndef NCURSESPLUSPLUS_TERMINAL_TERMINAL_ERROR_HPP
#define NCURSESPLUSPLUS_TERMINAL_TERMINAL_ERROR_HPP
#include <stdexcept>

namespace npp {

/// Runtime Terminal Error
/** Used when a terminal does not support some feature. */
struct Terminal_error : std::runtime_error {
  using std::runtime_error::runtime_error;
};

}  // namespace npp
#endif  // NCURSESPLUSPLUS_TERMINAL_TERMINAL_ERROR_HPP
