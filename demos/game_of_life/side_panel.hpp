#ifndef NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_SIDE_PANEL_HPP
#define NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_SIDE_PANEL_HPP
#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/widget.hpp"
#include "ncursesplusplus/widget/widgets/accordion.hpp"

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
        this->MakeChild<Pattern_rulesets>({L"Examples", npp::Align::Left, line})
            .wrapped();

    Files_box& files =
        this->MakeChild<Files>({L"Files", npp::Align::Left, line}).wrapped();

    Settings& settings_accordion =
        this->MakeChild<Settings>({L"Controls", npp::Align::Left, line});

    Controls_box& settings = settings_accordion.wrapped();

    Status_box& status =
        this->MakeChild<Status>({L"Status", npp::Align::Left, line}).wrapped();

    npp::Widget& empty_space = this->MakeChild();

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
#endif  // NCURSESPLUSPLUS_DEMOS_GAME_OF_LIFE_SIDE_PANEL_HPP
