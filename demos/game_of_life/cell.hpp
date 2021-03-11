#ifndef NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_CELL_HPP
#define NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_CELL_HPP
#include <cstdint>

namespace gol {

struct Cell {
    using Age_t = std::uint32_t;
    Age_t age{0};
};

}  // namespace gol
#endif  // NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_CELL_HPP
