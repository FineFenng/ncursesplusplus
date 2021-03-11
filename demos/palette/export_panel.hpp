#ifndef NCURSESPLUSPLUS_DEMOS_PALETTE_EXPORT_PANEL_HPP
#define NCURSESPLUSPLUS_DEMOS_PALETTE_EXPORT_PANEL_HPP
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/widgets/confirm_button.hpp"
#include "ncursesplusplus/widget/widgets/line_edit.hpp"

namespace palette {

/// Holds interface to export the Palette as a C++ header file.
class Export_panel : public npp::layout::Vertical<> {
   public:
    Export_panel();

   private:
    npp::Line_edit& name_edit_ = make_child<npp::Line_edit>(L"Enter name...");

    npp::Confirm_button& export_btn_ =
        make_child<npp::Confirm_button>(L"Export Palette");
};

}  // namespace palette
#endif  // NCURSESPLUSPLUS_DEMOS_PALETTE_EXPORT_PANEL_HPP
