#ifndef DEMOS_GLYPH_PAINT_TRAIT_BOX_HPP
#define DEMOS_GLYPH_PAINT_TRAIT_BOX_HPP
#include "ncursesplusplus/painter/glyph_string.hpp"
#include "ncursesplusplus/painter/trait.hpp"
#include "ncursesplusplus/widget/layouts/horizontal.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/widgets/checkbox.hpp"
#include "ncursesplusplus/widget/widgets/label.hpp"

namespace paint {

class Trait_box : public npp::layout::Vertical<> {
   private:
    using Checkbox = npp::HCheckbox_label;

    struct Top_row : npp::layout::Horizontal<Checkbox> {
        Checkbox& bold_box = make_child({L"Bold" | npp::Trait::Bold});
        Checkbox& dim_box  = make_child({L"Dim" | npp::Trait::Dim});
    };

   public:
    Top_row& top_row = make_child<Top_row>();
    Checkbox& inverse_box =
        make_child<Checkbox>({L"Inverse" | npp::Trait::Inverse});
    Checkbox& italic_box =
        make_child<Checkbox>({L"Italic" | npp::Trait::Italic});
    Checkbox& underline_box =
        make_child<Checkbox>({L"Underline" | npp::Trait::Underline});

   public:
    Trait_box() { this->height_policy.fixed(4); }
};

}  // namespace paint
#endif  // DEMOS_GLYPH_PAINT_TRAIT_BOX_HPP
