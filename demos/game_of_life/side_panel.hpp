#ifndef TERMOX_DEMOS_GAME_OF_LIFE_SIDE_PANEL_HPP
#define TERMOX_DEMOS_GAME_OF_LIFE_SIDE_PANEL_HPP
#include <termox/painter/color.hpp>
#include <termox/widget/layouts/vertical.hpp>
#include <termox/widget/pipe.hpp>
#include <termox/widget/widget.hpp>
#include <termox/widget/widgets/accordion.hpp>

#include "colors.hpp"
#include "controls_box.hpp"
#include "files_box.hpp"
#include "patterns_rulesets_box.hpp"
#include "status_box.hpp"

namespace gol {

struct Side_panel : npp::layout::Vertical<> {
   private:
    using Pattern_rulesets = npp::VAccordion<Patterns_rulesets_box>;
    using Files            = npp::VAccordion<Files_box>;
    using Settings         = npp::VAccordion<Controls_box>;
    using Status           = npp::VAccordion<Status_box>;

   public:
    Patterns_rulesets_box& patterns_rulesets =
        this->make_child<Pattern_rulesets>({L"Examples", npp::Align::Left, line})
            .wrapped();

    Files_box& files =
        this->make_child<Files>({L"Files", npp::Align::Left, line}).wrapped();

    Settings& settings_accordion =
        this->make_child<Settings>({L"Controls", npp::Align::Left, line});

    Controls_box& settings = settings_accordion.wrapped();

    Status_box& status =
        this->make_child<Status>({L"Status", npp::Align::Left, line}).wrapped();

    npp::Widget& empty_space = this->make_child();

   public:
    Side_panel()
    {
        using namespace npp;
        using namespace npp::pipe;

        *this | fixed_width(16uL);

        empty_space | expanding_height(0uL);
        settings_accordion.expand();
    }

   private:
    inline static auto const line = L'─' | fg(color::Light_green);
};
}  // namespace gol
#endif  // TERMOX_DEMOS_GAME_OF_LIFE_SIDE_PANEL_HPP
