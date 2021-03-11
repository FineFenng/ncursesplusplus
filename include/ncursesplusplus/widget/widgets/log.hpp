#ifndef NCURSESPLUSPLUS_WIDGET_WIDGETS_LOG_HPP
#define NCURSESPLUSPLUS_WIDGET_WIDGETS_LOG_HPP
#include <signals_light/signal.hpp"

#include "ncursesplusplus/system/key.hpp"
#include "ncursesplusplus/widget/widgets/text_display.hpp"
#include "ncursesplusplus/widget/widgets/textbox.hpp"

namespace npp {
class Glyph_string;

class Log : public Textbox {
   public:
    void post_message(Glyph_string message);

   protected:
    auto key_press_event(Key k) -> bool override;

    using Text_display::append;
    using Text_display::erase;
    using Text_display::insert;
    using Text_display::pop_back;
    using Text_display::set_contents;
};

/// Helper function to create an instance.
template <typename... Args>
auto log(Args&&... args) -> std::unique_ptr<Log>
{
    return std::make_unique<Log>(std::forward<Args>(args)...);
}

}  // namespace npp

namespace npp::slot {

auto post_message(Log& log) -> sl::Slot<void(Glyph_string)>;
auto post_message(Log& log, Glyph_string const& message) -> sl::Slot<void()>;

}  // namespace npp::slot
#endif  // NCURSESPLUSPLUS_WIDGET_WIDGETS_LOG_HPP
