#ifndef NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_MAKE_BREAK_HPP
#define NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_MAKE_BREAK_HPP
#include <memory>

#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/widget.hpp"

#include "colors.hpp"

namespace gol {

inline auto make_break() -> std::unique_ptr<npp::Widget>
{
    using namespace npp::pipe;
    return npp::widget() | fixed_height(1uL) | wallpaper(L'─' | fg(color::Teal));
}

}  // namespace gol
#endif  // NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_MAKE_BREAK_HPP
