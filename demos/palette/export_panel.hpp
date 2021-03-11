#ifndef TERMOX_DEMOS_PALETTE_EXPORT_PANEL_HPP
#define TERMOX_DEMOS_PALETTE_EXPORT_PANEL_HPP
#include <termox/widget/layouts/vertical.hpp>
#include <termox/widget/widgets/confirm_button.hpp>
#include <termox/widget/widgets/line_edit.hpp>

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
#endif  // TERMOX_DEMOS_PALETTE_EXPORT_PANEL_HPP
