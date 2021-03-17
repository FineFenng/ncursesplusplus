#include <string>

#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/widget/align.hpp"
#include "ncursesplusplus/widget/array.hpp"
#include "ncursesplusplus/widget/layouts/float.hpp"
#include "ncursesplusplus/widget/layouts/horizontal.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/tuple.hpp"
#include "ncursesplusplus/widget/widgets/button.hpp"
#include "ncursesplusplus/widget/widgets/checkbox.hpp"
#include "ncursesplusplus/widget/widgets/cycle_stack.hpp"
#include "ncursesplusplus/widget/widgets/label.hpp"
#include "ncursesplusplus/widget/widgets/line_edit.hpp"
#include "ncursesplusplus/widget/widgets/notify_light.hpp"

// Wall of Checkboxes - Update a count of checked boxes.
class Wall : public npp::layout::Vertical<> {
   private:
    static auto constexpr vcount = 20;
    static auto constexpr hcount = 40;

   public:
    using Checkboxes =
        npp::Array<npp::layout::Horizontal<
                      npp::Array<npp::layout::Vertical<npp::Checkbox1>, vcount>>,
                  hcount>;

    class Count : public npp::HLabel {
       public:
        Count() : npp::HLabel{L"Count: 0"} {}

       public:
        void increment()
        {
            ++count_;
            this->npp::HLabel::set_text("Count: " + std::to_string(count_));
        }

        void decrement()
        {
            --count_;
            this->npp::HLabel::set_text("Count: " + std::to_string(count_));
        }

       private:
        int count_ = 0;
    };

   public:
    Checkboxes& boxes_ = make_child<Checkboxes>();
    Count& count       = make_child<Count>();

   public:
    Wall()
    {
        using namespace npp::pipe;
        count | fixed_height(1);

        boxes_ | children() | fixed_width(1);
        for (auto& row : boxes_.get_children()) {
            for (auto& child : row.get_children()) {
                child.checked.connect([this] { count.increment(); });
                child.unchecked.connect([this] { count.decrement(); });
            }
        }
    }
};

/// horizontal labeled checkboxes in all initial configurations
// use both h and v checkboxes
struct Horizontals : public npp::layout::Vertical<> {
    Horizontals()
    {
        using namespace npp;
        this->make_child<HCheckbox1_label>({L"Label!"});
        this->make_child<HCheckbox_label>({L"Label!"});
        this->make_child<HLabel_checkbox1>({L"Label!"});
        this->make_child<HLabel_checkbox>({L"Label!"});
        this->make_child<HCheckbox2_label>({L"Label!"});
        this->make_child<HLabel_checkbox2>({L"Label!"});
        this->make_child<HCheckbox3_label>({L"Label!"});
        this->make_child<HLabel_checkbox4>({L"Label!"});
        this->make_child<HCheckbox7_label>({L"Label!"});
        this->make_child<HLabel_checkbox8>({L"Label!"});
        this->make_child<HCheckbox11_label>({L"Label!"});
        this->make_child<HLabel_checkbox12>({L"Label!"});
        this->make_child<HCheckbox15_label>({L"Label!"});
        this->make_child<HLabel_checkbox16>({L"Label!"});
        this->make_child<HCheckbox19_label>({L"Label!"});
        this->make_child<HLabel_checkbox19>({L"Label!"});
        this->make_child<HCheckbox20_label>({L"Label!"});
        this->make_child<HLabel_checkbox20>({L"Label!"});
    }
};

/// vertical labeled checkboxes in all initial configurations
// use both h and v checkboxes
struct Verticals : public npp::layout::Horizontal<> {
    Verticals()
    {
        using namespace npp;
        this->make_child<VCheckbox1_label>({L"Label!"});
        this->make_child<VCheckbox_label>({L"Label!"});
        this->make_child<VLabel_checkbox1>({L"Label!"});
        this->make_child<VLabel_checkbox>({L"Label!"});
        this->make_child<VCheckbox2_label>({L"Label!"});
        this->make_child<VLabel_checkbox2>({L"Label!"});
        this->make_child<VCheckbox5_label>({L"Label!"});
        this->make_child<VLabel_checkbox6>({L"Label!"});
        this->make_child<VCheckbox9_label>({L"Label!"});
        this->make_child<VLabel_checkbox10>({L"Label!"});
        this->make_child<VCheckbox13_label>({L"Label!"});
        this->make_child<VLabel_checkbox14>({L"Label!"});
        this->make_child<VCheckbox17_label>({L"Label!"});
        this->make_child<VLabel_checkbox18>({L"Label!"});
        this->make_child<VCheckbox19_label>({L"Label!"});
        this->make_child<VLabel_checkbox19>({L"Label!"});
        this->make_child<VCheckbox20_label>({L"Label!"});
        this->make_child<VLabel_checkbox20>({L"Label!"});
    }
};

// Parameters box to modify look and lock and display running list of signals
// emitted, you only need a single checkbox for this. no need for a labeled cb
class Runtime : public npp::layout::Horizontal<> {
   private:
    class Checkbox_options : public npp::layout::Vertical<> {
       public:
        template <template <typename> typename Layout_t>
        Checkbox_options(npp::Checkbox<Layout_t>& cb)
        {
            using namespace npp::pipe;
            using namespace npp;

            *this | fixed_width(24);

            check_btn | fixed_height(1) | bg(Color::Dark_gray);
            uncheck_btn | fixed_height(1) | bg(Color::Dark_blue);
            toggle_btn | fixed_height(1) | bg(Color::Dark_gray);

            checked_display.wrapped | fixed_width(6) | bg(Color::Dark_gray);
            unchecked_display.wrapped | fixed_width(6) | bg(Color::Dark_gray);

            check_btn | on_press([&cb] { cb.check(); });
            uncheck_btn | on_press([&cb] { cb.uncheck(); });
            toggle_btn | on_press([&cb] { cb.toggle(); });

            lock_cb.checkbox | on_check([&cb] { cb.lock(); });
            lock_cb.checkbox | on_uncheck([&cb] { cb.unlock(); });

            unchecked_display.wrapped.contents_modified.connect(
                [&cb](auto const& contents) {
                    cb.set_display({cb.get_display().checked, contents});
                });
            checked_display.wrapped.contents_modified.connect(
                [&cb](auto const& contents) {
                    cb.set_display({contents, cb.get_display().unchecked});
                });

            cb | on_check([this] { checked_lb.wrapped.emit(); });
            cb | on_uncheck([this] { unchecked_lb.wrapped.emit(); });
            cb | on_toggle([this] { toggled_lb.wrapped.emit(); });
        }

       private:
        using Checkbox  = npp::HCheckbox20_label;
        using Edit      = npp::HLabel_left<npp::Line_edit>;
        using Light_box = npp::HLabel_right<npp::Notify_light>;

        // TODO make a Widget with template template layout parameter
        struct Divider : public Widget {
            Divider()
            {
                using namespace npp::pipe;
                *this | fixed_height(1) |
                    wallpaper(L'─' | fg(npp::Color::Light_gray));
            }
        };

       public:
        npp::Button& check_btn   = this->MakeChild<npp::Button>(L"Check");
        npp::Button& uncheck_btn = this->MakeChild<npp::Button>(L"Uncheck");
        npp::Button& toggle_btn  = this->MakeChild<npp::Button>(L"Toggle");

        Divider& div1     = this->MakeChild<Divider>();
        Checkbox& lock_cb = this->MakeChild<Checkbox>({L"Lock"});
        Divider& div2     = this->MakeChild<Divider>();

        Edit& unchecked_display = this->MakeChild<Edit>(
            Edit::Parameters{L"Unchecked Display", npp::Align::Left},
            L"☐");
        Edit& checked_display = this->MakeChild<Edit>(
            Edit::Parameters{L"Checked   Display", npp::Align::Left},
            L"☒");

        Divider& div3 = this->MakeChild<Divider>();

        Light_box& checked_lb = this->MakeChild<Light_box>(
            Light_box::Parameters{L"`checked`   Signal"},
            npp::Notify_light::Display{npp::Color::Yellow, npp::Color::Dark_gray},
            npp::Notify_light::Duration_t{350});
        Light_box& unchecked_lb = this->MakeChild<Light_box>(
            Light_box::Parameters{L"`unchecked` Signal"},
            npp::Notify_light::Display{npp::Color::Yellow, npp::Color::Dark_gray},
            npp::Notify_light::Duration_t{350});
        Light_box& toggled_lb = this->MakeChild<Light_box>(
            Light_box::Parameters{L"`toggled`   Signal"},
            npp::Notify_light::Display{npp::Color::Yellow, npp::Color::Dark_gray},
            npp::Notify_light::Duration_t{350});
    };

   public:
    Runtime()
    {
        using namespace npp::pipe;
        divider | fixed_width(1) | wallpaper(L'│' | fg(npp::Color::Light_gray));
    }

   private:
    using Checkbox = npp::Float_2d<npp::Checkbox1>;

   private:
    Checkbox& checkbox = this->MakeChild<Checkbox>();
    Widget& divider    = this->MakeChild();
    Checkbox_options& options =
        this->MakeChild<Checkbox_options>(checkbox.widget.widget);
};

class Test : public npp::Cycle_stack<> {
   public:
    Test()
    {
        this->make_page<Wall>(L"Wall");
        this->make_page<Horizontals>(L"Labeled Horizontals");
        this->make_page<Verticals>(L"Labeled Verticals");
        // clang-format off
        this->append_page(L"Checkbox() Helpers",
                          npp::layout::vertical(
                                  npp::layout::horizontal(
                                          npp::checkbox1(), npp::checkbox2(), npp::checkbox3(),
                                          npp::checkbox4(), npp::checkbox5(), npp::checkbox6(),
                                          npp::checkbox7(), npp::checkbox8(), npp::checkbox9(),
                                          npp::checkbox10(), npp::checkbox11(), npp::checkbox12(),
                                          npp::checkbox13(), npp::checkbox14(), npp::checkbox15(),
                                          npp::checkbox16(), npp::checkbox17(), npp::checkbox18(),
                                          npp::checkbox19(), npp::checkbox20()
                ),
                                  npp::layout::horizontal(
                                          npp::checkbox1({}), npp::checkbox2({}), npp::checkbox3({}),
                                          npp::checkbox4({}), npp::checkbox5({}), npp::checkbox6({}),
                                          npp::checkbox7({}), npp::checkbox8({}), npp::checkbox9({}),
                                          npp::checkbox10({}), npp::checkbox11({}), npp::checkbox12({}),
                                          npp::checkbox13({}), npp::checkbox14({}), npp::checkbox15({}),
                                          npp::checkbox16({}), npp::checkbox17({}), npp::checkbox18(),
                                          npp::checkbox19({}), npp::checkbox20({})
                ),
                                  npp::layout::horizontal(
                                          npp::checkbox1({npp::Checkbox1::State::Checked, false}),
                                          npp::checkbox2({npp::Checkbox2::State::Checked, false}),
                                          npp::checkbox3({npp::Checkbox3::State::Checked, false}),
                                          npp::checkbox4({npp::Checkbox4::State::Checked, false}),
                                          npp::checkbox5({npp::Checkbox5::State::Checked, false}),
                                          npp::checkbox6({npp::Checkbox6::State::Checked, false}),
                                          npp::checkbox7({npp::Checkbox7::State::Checked, false}),
                                          npp::checkbox8({npp::Checkbox8::State::Checked, false}),
                                          npp::checkbox9({npp::Checkbox9::State::Checked, false}),
                                          npp::checkbox10({npp::Checkbox10::State::Checked, false}),
                                          npp::checkbox11({npp::Checkbox11::State::Checked, false}),
                                          npp::checkbox12({npp::Checkbox12::State::Checked, false}),
                                          npp::checkbox13({npp::Checkbox13::State::Checked, false}),
                                          npp::checkbox14({npp::Checkbox14::State::Checked, false}),
                                          npp::checkbox15({npp::Checkbox15::State::Checked, false}),
                                          npp::checkbox16({npp::Checkbox16::State::Checked, false}),
                                          npp::checkbox17({npp::Checkbox17::State::Checked, false}),
                                          npp::checkbox18({npp::Checkbox18::State::Checked, false}),
                                          npp::checkbox19({npp::Checkbox19::State::Checked, false}),
                                          npp::checkbox20({npp::Checkbox20::State::Checked, false})
                ),
                                  npp::layout::horizontal(
                                          npp::checkbox1(npp::Checkbox1::State::Unchecked, true),
                                          npp::checkbox2(npp::Checkbox2::State::Unchecked, true),
                                          npp::checkbox3(npp::Checkbox3::State::Unchecked, true),
                                          npp::checkbox4(npp::Checkbox4::State::Unchecked, true),
                                          npp::checkbox5(npp::Checkbox5::State::Unchecked, true),
                                          npp::checkbox6(npp::Checkbox6::State::Unchecked, true),
                                          npp::checkbox7(npp::Checkbox7::State::Unchecked, true),
                                          npp::checkbox8(npp::Checkbox8::State::Unchecked, true),
                                          npp::checkbox9(npp::Checkbox9::State::Unchecked, true),
                                          npp::checkbox10(npp::Checkbox10::State::Unchecked, true),
                                          npp::checkbox11(npp::Checkbox11::State::Unchecked, true),
                                          npp::checkbox12(npp::Checkbox12::State::Unchecked, true),
                                          npp::checkbox13(npp::Checkbox13::State::Unchecked, true),
                                          npp::checkbox14(npp::Checkbox14::State::Unchecked, true),
                                          npp::checkbox15(npp::Checkbox15::State::Unchecked, true),
                                          npp::checkbox16(npp::Checkbox16::State::Unchecked, true),
                                          npp::checkbox17(npp::Checkbox17::State::Unchecked, true),
                                          npp::checkbox18(npp::Checkbox18::State::Unchecked, true),
                                          npp::checkbox19(npp::Checkbox19::State::Unchecked, true),
                                          npp::checkbox20(npp::Checkbox20::State::Unchecked, true)
                )
            )
        );
        this->append_page(L"Labeled Horizontal Helpers - No Parameters",
                          npp::layout::vertical(
                                  npp::hcheckbox1_label(),
                                  npp::hcheckbox_label(),
                                  npp::hlabel_checkbox1(),
                                  npp::hlabel_checkbox(),
                                  npp::hcheckbox2_label(),
                                  npp::hlabel_checkbox2(),
                                  npp::hcheckbox3_label(),
                                  npp::hlabel_checkbox4(),
                                  npp::hcheckbox7_label(),
                                  npp::hlabel_checkbox8(),
                                  npp::hcheckbox11_label(),
                                  npp::hlabel_checkbox12(),
                                  npp::hcheckbox15_label(),
                                  npp::hlabel_checkbox16(),
                                  npp::hcheckbox19_label(),
                                  npp::hlabel_checkbox19(),
                                  npp::hcheckbox20_label(),
                                  npp::hlabel_checkbox20()
            )
        );
        this->append_page(L"Labeled Horizontal Helpers - Label Parameters",
                          npp::layout::vertical(
                                  npp::hcheckbox1_label({L"Label!", npp::Align::Right}),
                                  npp::hcheckbox_label({L"Label!", npp::Align::Left}),
                                  npp::hlabel_checkbox1({L"Label!", npp::Align::Center}),
                                  npp::hlabel_checkbox({L"Label!", npp::Align::Right}),
                                  npp::hcheckbox2_label({L"Label!", npp::Align::Left}),
                                  npp::hlabel_checkbox2({L"Label!", npp::Align::Center}),
                                  npp::hcheckbox3_label({L"Label!", npp::Align::Right}),
                                  npp::hlabel_checkbox4({L"Label!", npp::Align::Left}),
                                  npp::hcheckbox7_label({L"Label!", npp::Align::Center}),
                                  npp::hlabel_checkbox8({L"Label!", npp::Align::Right}),
                                  npp::hcheckbox11_label({L"Label!", npp::Align::Left}),
                                  npp::hlabel_checkbox12({L"Label!", npp::Align::Center}),
                                  npp::hcheckbox15_label({L"Label!", npp::Align::Right}),
                                  npp::hlabel_checkbox16({L"Label!", npp::Align::Left}),
                                  npp::hcheckbox19_label({L"Label!", npp::Align::Center}),
                                  npp::hlabel_checkbox19({L"Label!", npp::Align::Right}),
                                  npp::hcheckbox20_label({L"Label!", npp::Align::Left}),
                                  npp::hlabel_checkbox20({L"Label!", npp::Align::Center})
            )
        );
        this->append_page(
                L"Labeled Horizontal Helpers - Label and Checkbox Parameters",
                npp::layout::vertical(
                        npp::hcheckbox1_label({L"Label!", npp::Align::Right},
                                              {npp::Checkbox1::State::Checked, false}),
                        npp::hcheckbox_label({L"Label!", npp::Align::Left},
                                             {npp::Checkbox1::State::Unchecked, true}),
                        npp::hlabel_checkbox1({L"Label!", npp::Align::Center},
                                              {npp::Checkbox1::State::Checked, false}),
                        npp::hlabel_checkbox({L"Label!", npp::Align::Right},
                                             {npp::Checkbox1::State::Unchecked, false}),
                        npp::hcheckbox2_label({L"Label!", npp::Align::Left},
                                              {npp::Checkbox2::State::Checked, true}),
                        npp::hlabel_checkbox2({L"Label!", npp::Align::Center},
                                              {npp::Checkbox2::State::Unchecked, false}),
                        npp::hcheckbox3_label({L"Label!", npp::Align::Right},
                                              {npp::Checkbox3::State::Checked, false}),
                        npp::hlabel_checkbox4({L"Label!", npp::Align::Left},
                                              {npp::Checkbox4::State::Unchecked, false}),
                        npp::hcheckbox7_label({L"Label!", npp::Align::Center},
                                              {npp::Checkbox7::State::Checked, true}),
                        npp::hlabel_checkbox8({L"Label!", npp::Align::Right},
                                              {npp::Checkbox8::State::Unchecked, false}),
                        npp::hcheckbox11_label({L"Label!", npp::Align::Left},
                                               {npp::Checkbox11::State::Checked, false}),
                        npp::hlabel_checkbox12({L"Label!", npp::Align::Center},
                                               {npp::Checkbox12::State::Unchecked, true}),
                        npp::hcheckbox15_label({L"Label!", npp::Align::Right},
                                               {npp::Checkbox15::State::Checked, false}),
                        npp::hlabel_checkbox16({L"Label!", npp::Align::Left},
                                               {npp::Checkbox16::State::Unchecked, true}),
                        npp::hcheckbox19_label({L"Label!", npp::Align::Center},
                                               {npp::Checkbox19::State::Checked, false}),
                        npp::hlabel_checkbox19({L"Label!", npp::Align::Right},
                                               {npp::Checkbox19::State::Unchecked, true}),
                        npp::hcheckbox20_label({L"Label!", npp::Align::Left},
                                               {npp::Checkbox20::State::Checked, false}),
                        npp::hlabel_checkbox20({L"Label!", npp::Align::Center},
                                               {npp::Checkbox20::State::Checked, true})
            )
        );
        this->append_page(L"Labeled Vertical Helpers - No Parameters",
                          npp::layout::horizontal(
                                  npp::vcheckbox1_label(),
                                  npp::vcheckbox_label(),
                                  npp::vlabel_checkbox1(),
                                  npp::vlabel_checkbox(),
                                  npp::vcheckbox2_label(),
                                  npp::vlabel_checkbox2(),
                                  npp::vcheckbox5_label(),
                                  npp::vlabel_checkbox6(),
                                  npp::vcheckbox9_label(),
                                  npp::vlabel_checkbox10(),
                                  npp::vcheckbox13_label(),
                                  npp::vlabel_checkbox14(),
                                  npp::vcheckbox17_label(),
                                  npp::vlabel_checkbox18(),
                                  npp::vcheckbox19_label(),
                                  npp::vlabel_checkbox19(),
                                  npp::vcheckbox20_label(),
                                  npp::vlabel_checkbox20()
            )
        );
        this->append_page(L"Labeled Vertical Helpers - Label Parameters",
                          npp::layout::horizontal(
                                  npp::vcheckbox1_label  ({L"Label!", npp::Align::Right}),
                                  npp::vcheckbox_label   ({L"Label!", npp::Align::Left}),
                                  npp::vlabel_checkbox1  ({L"Label!", npp::Align::Center}),
                                  npp::vlabel_checkbox   ({L"Label!", npp::Align::Right}),
                                  npp::vcheckbox2_label  ({L"Label!", npp::Align::Left}),
                                  npp::vlabel_checkbox2  ({L"Label!", npp::Align::Center}),
                                  npp::vcheckbox5_label  ({L"Label!", npp::Align::Right}),
                                  npp::vlabel_checkbox6  ({L"Label!", npp::Align::Left}),
                                  npp::vcheckbox9_label  ({L"Label!", npp::Align::Center}),
                                  npp::vlabel_checkbox10 ({L"Label!", npp::Align::Right}),
                                  npp::vcheckbox13_label ({L"Label!", npp::Align::Left}),
                                  npp::vlabel_checkbox14 ({L"Label!", npp::Align::Center}),
                                  npp::vcheckbox17_label ({L"Label!", npp::Align::Right}),
                                  npp::vlabel_checkbox18 ({L"Label!", npp::Align::Left}),
                                  npp::vcheckbox19_label ({L"Label!", npp::Align::Center}),
                                  npp::vlabel_checkbox19 ({L"Label!", npp::Align::Right}),
                                  npp::vcheckbox20_label ({L"Label!", npp::Align::Left}),
                                  npp::vlabel_checkbox20 ({L"Label!", npp::Align::Center})
            )
        );
        this->append_page(
                L"Labeled Vertical Helpers - Label and Checkbox Parameters",
                npp::layout::horizontal(
                        npp::vcheckbox1_label({L"Label!", npp::Align::Right},
                                              {npp::Checkbox1::State::Checked, false}),
                        npp::vcheckbox_label({L"Label!", npp::Align::Left},
                                             {npp::Checkbox1::State::Unchecked, true}),
                        npp::vlabel_checkbox1({L"Label!", npp::Align::Center},
                                              {npp::Checkbox1::State::Checked, false}),
                        npp::vlabel_checkbox({L"Label!", npp::Align::Right},
                                             {npp::Checkbox1::State::Unchecked, false}),
                        npp::vcheckbox2_label({L"Label!", npp::Align::Left},
                                              {npp::Checkbox2::State::Checked, true}),
                        npp::vlabel_checkbox2({L"Label!", npp::Align::Center},
                                              {npp::Checkbox2::State::Unchecked, false}),
                        npp::vcheckbox5_label({L"Label!", npp::Align::Right},
                                              {npp::Checkbox5::State::Checked, false}),
                        npp::vlabel_checkbox6({L"Label!", npp::Align::Left},
                                              {npp::Checkbox6::State::Unchecked, false}),
                        npp::vcheckbox9_label({L"Label!", npp::Align::Center},
                                              {npp::Checkbox9::State::Checked, true}),
                        npp::vlabel_checkbox10({L"Label!", npp::Align::Right},
                                               {npp::Checkbox10::State::Unchecked, false}),
                        npp::vcheckbox13_label({L"Label!", npp::Align::Left},
                                               {npp::Checkbox13::State::Checked, false}),
                        npp::vlabel_checkbox14({L"Label!", npp::Align::Center},
                                               {npp::Checkbox14::State::Unchecked, true}),
                        npp::vcheckbox17_label({L"Label!", npp::Align::Right},
                                               {npp::Checkbox17::State::Checked, false}),
                        npp::vlabel_checkbox18({L"Label!", npp::Align::Left},
                                               {npp::Checkbox18::State::Unchecked, true}),
                        npp::vcheckbox19_label({L"Label!", npp::Align::Center},
                                               {npp::Checkbox19::State::Checked, false}),
                        npp::vlabel_checkbox19({L"Label!", npp::Align::Right},
                                               {npp::Checkbox19::State::Unchecked, true}),
                        npp::vcheckbox20_label({L"Label!", npp::Align::Left},
                                               {npp::Checkbox20::State::Checked, false}),
                        npp::vlabel_checkbox20({L"Label!", npp::Align::Center},
                                               {npp::Checkbox20::State::Checked, true})
            )
        );
        // clang-format on
        this->make_page<Runtime>(L"Runtime");
    }
};

int main() { return npp::System{}.run<Test>(); }
