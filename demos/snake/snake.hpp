#ifndef NCURSESPLUSPLUS_DEMOS_SNAKE_SNAKE_HPP
#define NCURSESPLUSPLUS_DEMOS_SNAKE_SNAKE_HPP
#include <algorithm>
#include <chrono>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <signals_light/signal.hpp"

#include "ncursesplusplus/painter/color.hpp"
#include "ncursesplusplus/painter/dynamic_colors.hpp"
#include "ncursesplusplus/painter/palette/stormy6.hpp"
#include "ncursesplusplus/painter/trait.hpp"
#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/system/key.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/widget/align.hpp"
#include "ncursesplusplus/widget/area.hpp"
#include "ncursesplusplus/widget/layouts/float.hpp"
#include "ncursesplusplus/widget/layouts/horizontal.hpp"
#include "ncursesplusplus/widget/layouts/vertical.hpp"
#include "ncursesplusplus/widget/pipe.hpp"
#include "ncursesplusplus/widget/point.hpp"
#include "ncursesplusplus/widget/widget.hpp"
#include "ncursesplusplus/widget/widgets/confirm_button.hpp"
#include "ncursesplusplus/widget/widgets/cycle_box.hpp"
#include "ncursesplusplus/widget/widgets/label.hpp"
#include "ncursesplusplus/widget/widgets/text_display.hpp"
#include "ncursesplusplus/widget/widgets/toggle_button.hpp"

namespace snake {

/// A field of apples, stored as Points to their location within a given Area.
class Apple_field {
   public:
    /// Initialize the field with some % of cells as apples.
    template <typename Range>
    void initialize(npp::Area const& a, Range const& mask)
    {
        this->clear();
        this->resize(a);
        auto constexpr coverage = .004;
        auto const total        = a.width * a.height * coverage;
        for (auto i = 0; i < total; ++i)
            this->create_apple(mask);
    }

    /// Add a new apple at a random point within the area.
    /** Will not add the apple over an existing apple, or over any point within
     *  the given \p mask. */
    template <typename Range>
    void create_apple(Range const& mask)
    {
        if (area_.width == 0uL || area_.height == 0uL)
            return;
        if (points_.size() == ((area_.width * area_.height) - mask.size()))
            return;
        auto p = npp::Point{0uL, 0uL};
        do {
            p.x = random_index(area_.width - 1uL);
            p.y = random_index(area_.height - 1uL);
        } while (points_.count(p) > 0 && !contain(mask, p));
        points_.insert(p);
    }

    /// Remove apple at point \p if one exists
    void remove(npp::Point const& p) { points_.erase(p); }

    /// Return true if there is an apple at \p p.
    auto contains(npp::Point const& p) const -> bool
    {
        return points_.count(p) > 0;
    }

    /// Return iterator to the first apple Point in the field.
    auto begin() const { return std::cbegin(points_); }

    /// Return iterator to one past-the-last apple Point in the field.
    auto end() const { return std::cend(points_); }

    /// Return the number of apples in the field.
    auto size() const -> std::size_t { return points_.size(); }

    /// Return the field's bounding Area.
    auto area() const -> npp::Area const& { return area_; }

    /// Resize the apple field, remove apples outside of the new boundary.
    void resize(npp::Area const& a)
    {
        area_ = a;
        for (auto point = std::begin(points_); point != std::end(points_);) {
            if (point->x >= area_.width || point->y >= area_.height)
                point = points_.erase(point);
            else
                std::advance(point, 1);
        }
    }

    /// Removes all apples, does not resize.
    void clear() { points_.Clear(); }

   private:
    std::set<npp::Point> points_;
    npp::Area area_;

   private:
    /// Return random value between [0, max]
    static auto random_index(std::size_t max) -> std::size_t
    {
        static auto gen = std::mt19937{std::random_device{}()};
        auto dist       = std::uniform_int_distribution<std::size_t>{0, max};
        return dist(gen);
    }

    /// Return true if \p p is found within \p r.
    template <typename Range>
    static auto contain(Range const& r, npp::Point const& p) -> bool
    {
        return std::find(std::cbegin(r), std::cend(r), p) != std::cend(r);
    }
};

class Snake {
   public:
    enum class Direction { Left, Right, Up, Down };

   public:
    /// Clear existing snake, create a new snake at \p p with Direction::Right.
    void initialize(npp::Point const& p)
    {
      points_.Clear();
        points_.push_back(p);
        direction_      = Direction::Right;
        next_direction_ = Direction::Right;
    }

    /// Allow direction changes.
    void enable_input() { has_input_ = true; }

    /// Disallow direction changes.
    void disable_input() { has_input_ = false; }

    /// Checks the direction isn't 180 degrees from the current before setting.
    void set_direction(Direction d)
    {
        if (has_input_ && !is_180_degrees(d, direction_))
            next_direction_ = d;
    }

    auto get_direction() const -> Direction { return direction_; }

    /// Adds a Point to the Snake's head in the current direction.
    void increment_head()
    {
        direction_       = next_direction_;
        auto const units = to_units(direction_);
        points_.push_back(
            {points_.back().x + units.first, points_.back().y + units.second});
    }

    /// Remove the tail Point from the Snake.
    void decrement_tail()
    {
        if (points_.empty())
            return;
        points_.erase(std::begin(points_));
    }

    /// Return the Point where the Snake's head is located.
    auto head() const -> npp::Point const& { return points_.back(); }

    /// Returns iterator to the tail of the snake.
    auto begin() const { return std::cbegin(points_); }

    /// Returns iterator to one past the head of the snake.
    auto end() const { return std::cend(points_); }

    /// Return the size of the snake, in cells.
    auto size() const { return points_.size(); }

    /// Return the point at snake segment \p i, where 0 is the tail.
    auto operator[](std::size_t i) const -> npp::Point const&
    {
        return points_[i];
    }

   private:
    // First point is the tail, last point is the head.
    std::vector<npp::Point> points_;
    Direction next_direction_ = Direction::Right;
    Direction direction_      = Direction::Right;
    bool has_input_           = false;

   private:
    /// Return true if the given Directions are at 180 degrees from each other.
    static auto is_180_degrees(Direction a, Direction b) -> bool
    {
        switch (a) {
            case Direction::Left: return b == Direction::Right;
            case Direction::Right: return b == Direction::Left;
            case Direction::Up: return b == Direction::Down;
            case Direction::Down: return b == Direction::Up;
        }
        return false;
    }

    /// Returns a pair of 0, -1, or +1, cooresponding to the direction \p d.
    static auto to_units(Direction d) -> std::pair<short, short>
    {
        switch (d) {
            case Direction::Left: return {-1, 0};
            case Direction::Right: return {1, 0};
            case Direction::Up: return {0, -1};
            case Direction::Down: return {0, 1};
        }
        return {0, 0};
    }
};

class Engine {
   private:
    enum class State { Dead, Apple, No_apple };

   public:
    Apple_field apples;
    Snake snake;

   public:
    sl::Signal<void()> game_over;
    sl::Signal<void(unsigned)> score;

   public:
    /// Set up the state for a new game.
    void reset(npp::Area a)
    {
        score_ = 0;
        score(score_);
        step_ = 0;
        snake.initialize({a.width / 2, a.height / 2});
        apples.initialize(a, snake);
    }

    /// Increment the game state by one step.
    void increment()
    {
        ++step_;
        snake.increment_head();
        switch (get_state(snake, apples)) {
            case State::Dead: game_over(); return;
            case State::Apple:
                ++score_;
                score(score_);
                apples.remove(snake.head());
                break;
            case State::No_apple: snake.decrement_tail(); break;
        }
        if (step_ % add_apple_at_ == 0)
            apples.create_apple(snake);
    }

    /// Return true is the game is over.
    auto is_game_over() const -> bool
    {
        return get_state(snake, apples) == State::Dead;
    }

    /// Return the current score.
    auto get_score() const -> unsigned { return score_; }

   private:
    unsigned score_ = 0;
    unsigned step_  = 0;

   private:
    static auto constexpr add_apple_at_ = 20;

   private:
    /// Return the current state of the game.
    static auto get_state(Snake const& snake, Apple_field const& apples)
        -> State
    {
        auto const& head = snake.head();
        if (head.x >= apples.area().width || head.y >= apples.area().height)
            return State::Dead;
        if (std::count(std::cbegin(snake), std::cend(snake), head) > 1)
            return State::Dead;

        if (apples.contains(head))
            return State::Apple;

        return State::No_apple;
    }
};

namespace color {

inline auto constexpr Fade    = npp::Color{16};
inline auto constexpr Rainbow = npp::Color{17};

inline auto constexpr Apple            = Fade;
inline auto constexpr Snake            = Rainbow;
inline auto constexpr Border           = npp::stormy6::Teal;
inline auto constexpr Instruction_fg   = npp::stormy6::White;
inline auto constexpr Instruction_bg   = npp::stormy6::Teal;
inline auto constexpr Start_bg         = npp::stormy6::Green;
inline auto constexpr Pause_bg         = npp::stormy6::Red;
inline auto constexpr Size_bg          = npp::stormy6::White;
inline auto constexpr Size_fg          = npp::stormy6::Black;
inline auto constexpr Score_bg         = npp::stormy6::White;
inline auto constexpr Score_fg         = npp::stormy6::Black;
inline auto constexpr Instruction_text = npp::stormy6::Orange;

}  // namespace color

inline auto const snake_palette = [] {
    auto pal = npp::stormy6::palette;
    pal.push_back({color::Fade, npp::ANSI{22},
                   npp::dynamic::fade<npp::dynamic::Sine>(0x7f9860, 0xa95a3f)});
    pal.push_back({color::Rainbow, npp::ANSI{23}, npp::dynamic::rainbow()});
    return pal;
}();

/// Main Game Widget
class Game_space : public npp::Widget {
   public:
    Game_space()
    {
        using namespace npp::pipe;
        *this | strong_focus();
        game_over.connect([this] { this->stop(); });
        score.connect([this](unsigned score) {
            auto constexpr limit    = std::chrono::milliseconds{40};
            auto constexpr interval = 7;
            if ((score % interval == 0) && period_ > limit)
                this->set_speed(reduce_by(period_, .05));
        });
    }

   public:
    /// Start the game from the current state.
    /** If the game is over, will reset the game state and start a new game. */
    void start()
    {
        if (too_small_) {
            this->stop();  // Emit signals to reset toggle button display.
            return;
        }
        if (engine_.is_game_over())
            this->reset();
        using namespace npp::pipe;
        engine_.snake.enable_input();
        *this | animate(period_);
        started();
    }

    /// Stop/pause the game.
    void stop()
    {
        using namespace npp::pipe;
        engine_.snake.disable_input();
        *this | disanimate();
        stopped();
    }

    /// Start the game if it is stopped, stop the game it started.
    void toggle()
    {
        if (this->is_animated())
            this->stop();
        else
            this->start();
    }

    /// Resize the playing surface, also resets the game state.
    void resize(npp::Area const& a)
    {
        using namespace npp::pipe;
        *this | fixed_width(a.width) | fixed_height(a.height);
        this->stop();
        engine_.reset(a);
        this->set_speed(initial_period_);
        too_small_ = this->is_too_small();
        this->update();
    }

    void reset()
    {
        this->stop();
        engine_.reset(engine_.apples.area());
        this->update();
        this->set_speed(initial_period_);
    }

    /// Set the amount of time between game steps.
    void set_speed(std::chrono::milliseconds period)
    {
        using namespace npp::pipe;
        if (this->is_animated())
            *this | disanimate() | animate(period);
        period_ = period;
    }

   protected:
    auto paint_event() -> bool override
    {
        if (too_small_)
            this->paint_size_message();
        else
            this->paint_game();
        return Widget::paint_event();
    }

    /// Change the direction of the snake.
    auto key_press_event(npp::Key k) -> bool override
    {
        switch (k) {
            using namespace npp;
            case Key::Space: this->toggle(); break;
            case Key::Arrow_left:
            case Key::h:
                engine_.snake.set_direction(Snake::Direction::Left);
                break;
            case Key::Arrow_right:
            case Key::l:
                engine_.snake.set_direction(Snake::Direction::Right);
                break;
            case Key::Arrow_up:
            case Key::k:
                engine_.snake.set_direction(Snake::Direction::Up);
                break;
            case Key::Arrow_down:
            case Key::j:
                engine_.snake.set_direction(Snake::Direction::Down);
                break;
            default: break;
        }
        return Widget::key_press_event(k);
    }

    /// Increment the game state.
    auto timer_event() -> bool override
    {
        engine_.increment();
        this->update();
        return Widget::timer_event();
    }

    /// Checks if the screen is too small to display the game state.
    auto resize_event(npp::Area new_size, npp::Area old_size) -> bool override
    {
        too_small_ = this->is_too_small();
        if (too_small_)
            this->stop();
        return Widget::resize_event(new_size, old_size);
    }

   private:
    inline static auto const initial_period_ = std::chrono::milliseconds{210};

    Engine engine_;
    std::chrono::milliseconds period_{initial_period_};
    bool too_small_ = false;

   public:
    sl::Signal<void()>& game_over     = engine_.game_over;
    sl::Signal<void(unsigned)>& score = engine_.score;
    sl::Signal<void()> started;
    sl::Signal<void()> stopped;

   private:
    auto is_too_small() -> bool
    {
        return this->width() < engine_.apples.area().width ||
               this->height() < engine_.apples.area().height;
    }

    void paint_size_message()
    {
        auto const w = [this] {
            auto const& widg_width = this->width();
            auto const& game_width = engine_.apples.area().width;
            return widg_width < game_width ? game_width - widg_width : 0;
        }();
        auto const h = [this] {
            auto const& widg_height = this->height();
            auto const& game_height = engine_.apples.area().height;
            return widg_height < game_height ? game_height - widg_height : 0;
        }();
        auto painter = npp::Painter{*this};
        painter.put(L"Screen is too small!", {0, 0});
        painter.put(L"Needs " + std::to_wstring(w) + L" more width.", {0, 1});
        painter.put(L"Needs " + std::to_wstring(h) + L" more height.", {0, 2});
    }

    void paint_game()
    {
        auto painter = npp::Painter{*this};
        for (auto const& p : engine_.apples) {
            // TODO store as static constexpr(?) glyph
            painter.put(L'@' | npp::Trait::Bold | fg(color::Apple), p);
        }

        // Body
        for (auto const& p : engine_.snake)
            painter.put(L'█' | fg(color::Snake), p);
        // TODO Store snake_body as glyph

        // Tail
        if (engine_.snake.size() > 3) {
            // TODO store as static glyphs
            painter.put(L'░' | fg(color::Snake), engine_.snake[0]);
            painter.put(L'▒' | fg(color::Snake), engine_.snake[1]);
            painter.put(L'▓' | fg(color::Snake), engine_.snake[2]);
        }
        else if (engine_.snake.size() == 3) {
            painter.put(L'▒' | fg(color::Snake), engine_.snake[0]);
            painter.put(L'▓' | fg(color::Snake), engine_.snake[1]);
        }
        else if (engine_.snake.size() == 2) {
            painter.put(L'▓' | fg(color::Snake), engine_.snake[0]);
        }

        // Head
        switch (engine_.snake.get_direction()) {
            case Snake::Direction::Up:
                painter.put(L'🭯' | fg(color::Snake), engine_.snake.head());
                break;
            case Snake::Direction::Down:
                painter.put(L'🭭' | fg(color::Snake), engine_.snake.head());
                break;
            case Snake::Direction::Left:
                painter.put(L'🭮' | fg(color::Snake), engine_.snake.head());
                break;
            case Snake::Direction::Right:
                painter.put(L'🭬' | fg(color::Snake), engine_.snake.head());
                break;
        }
    }

    /// Reduce \p duration by \p factor and return the result as ms.
    auto reduce_by(std::chrono::milliseconds duration, double factor)
        -> std::chrono::milliseconds
    {
        return duration - std::chrono::duration_cast<std::chrono::milliseconds>(
                              duration * factor);
    }
};

class Instructions : public npp::Text_display {
   public:
    Instructions() : Text_display{get_text()}
    {
        using namespace npp::pipe;
        *this | bg(color::Instruction_bg) | fg(color::Instruction_fg) |
            word_wrap(false) | align_center();
    }

   private:
    static auto get_text() -> npp::Glyph_string
    {
        using namespace npp;
        auto const standout = Brush{fg(color::Instruction_text), Trait::Bold};
        auto result         = Glyph_string{L"Start/Stop "};
      result.Append(L"Space Bar" | standout);
      result.Append(L" - Movement ");
      result.Append(L"Arrow Keys" | standout);
      result.Append(L" or ");
      result.Append(L"'hjkl'" | standout);
        return result;
    }
};

class Button_bar : public npp::layout::Horizontal<> {
   public:
    Button_bar()
    {
        using namespace npp::pipe;
        sizes_ | fixed_width(16uL);

        sizes_.label | bg(color::Size_bg) | fg(color::Size_fg);
        sizes_.div | bg(color::Size_bg) | fg(color::Size_fg);
        sizes_.cycle_box | bg(color::Size_bg) | fg(color::Size_fg);

        sizes_.cycle_box | no_focus();
        sizes_.cycle_box.add_option(L"Small").connect(
            [this] { size_change('s'); });
        sizes_.cycle_box.add_option(L"Medium").connect(
            [this] { size_change('m'); });
        sizes_.cycle_box.add_option(L"Large").connect(
            [this] { size_change('l'); });
        sizes_.cycle_box.add_option(L"X-Large").connect([this] {
            size_change('x');
        });

        start_pause_btns_ | fixed_width(15uL);
        start_pause_btns_.top | bg(color::Start_bg);
        start_pause_btns_.bottom | bg(color::Pause_bg);
    }

   public:
    void show_start_button() { start_pause_btns_.show_top(); }

    void show_pause_button() { start_pause_btns_.show_bottom(); }

   private:
    npp::Toggle_button& start_pause_btns_ =
        this->MakeChild<npp::Toggle_button>(L"Start" | npp::Trait::Bold,
                                            L"Pause" | npp::Trait::Bold);
    npp::Labeled_cycle_box& sizes_ =
        this->MakeChild<npp::Labeled_cycle_box>(L" Size");
    Instructions& instructions_ = this->MakeChild<Instructions>();

   public:
    sl::Signal<void()>& start = start_pause_btns_.top_pressed;
    sl::Signal<void()>& pause = start_pause_btns_.bottom_pressed;
    sl::Signal<void(char)> size_change;
};

class Score : public npp::layout::Horizontal<> {
   public:
    Score()
    {
        using namespace npp::pipe;
        *this | fixed_width(12uL) | children() | bg(color::Score_bg) |
            fg(color::Score_fg);
    }

   public:
    void set(unsigned score) { score_.set_text(std::to_string(score)); }

   private:
    npp::HLabel& label_ = this->MakeChild<npp::HLabel>({L"Score: "});
    npp::HLabel& score_ = this->MakeChild<npp::HLabel>({L"0", npp::Align::Right});

    // you could have a number_display widget, that is templated on the
    // number type, and has display options specific to numbers, then number
    // edit is built on top of this.
};

// tuple or pair
class Bottom_bar : public npp::layout::Horizontal<> {
   public:
    Button_bar& buttons = this->MakeChild<Button_bar>();
    Score& score        = this->MakeChild<Score>();
};

class Snake_game : public npp::layout::Vertical<> {
   private:
    using VFloat        = npp::Float<Game_space, npp::layout::Vertical>;
    using Floating_game = npp::Float<VFloat, npp::layout::Horizontal>;

   public:
    Snake_game()
    {
        using namespace npp::pipe;
        *this | direct_focus();

        bottom_ | fixed_height(1uL);
        bottom_.buttons.start.connect([this] { game_space_.start(); });
        bottom_.buttons.pause.connect([this] { game_space_.stop(); });

        auto constexpr s_size = npp::Area{60, 17};
        auto constexpr m_size = npp::Area{100, 27};
        auto constexpr l_size = npp::Area{160, 37};
        auto constexpr x_size = npp::Area{230, 47};

        bottom_.buttons.size_change.connect([=](char c) {
            switch (c) {
                case 's': game_space_.resize(s_size); break;
                case 'm': game_space_.resize(m_size); break;
                case 'l': game_space_.resize(l_size); break;
                case 'x': game_space_.resize(x_size); break;
            }
        });

        using namespace npp;
        using namespace npp::pipe;
        floating_game.buffer_1 | bg(color::Border);
        floating_game.buffer_2 | bg(color::Border);
        vfloat.buffer_1 | bg(color::Border);
        vfloat.buffer_2 | bg(color::Border);

        game_space_.resize(s_size);
        game_space_.score.connect(
            [this](unsigned score) { bottom_.score.set(score); });
        game_space_.started.connect(
            [this] { bottom_.buttons.show_pause_button(); });
        game_space_.stopped.connect(
            [this] { bottom_.buttons.show_start_button(); });
    }

   protected:
    auto focus_in_event() -> bool override
    {
        npp::System::terminal.set_palette(snake_palette);
        npp::System::set_focus(game_space_);
        return npp::layout::Vertical<>::focus_in_event();
    }

   private:
    Floating_game& floating_game = make_child<Floating_game>();
    VFloat& vfloat               = floating_game.widget;
    Game_space& game_space_      = vfloat.widget;
    Bottom_bar& bottom_          = make_child<Bottom_bar>();
};

}  // namespace snake
#endif  // NCURSESPLUSPLUS_DEMOS_SNAKE_SNAKE_HPP
