#Creating New Widgets

A new Widget can be defined by extending the `Widget` class. The main tool for
defining a Widget's behavior is overriding the event handler virtual functions
of `Widget`.

The constructor is commonly used to set [`Color`](colors.md)s and
[`Trait`](traits.md)s, the width and height [`Size_policy`](size-policy.md) ,
and the [`Focus_policy`](focus-policy.md), among others.

```cpp
class Canvas : public npp::Widget {
   public:
    Canvas()
    {
        using npp::Color;
        using namespace npp::pipe;
        *this | bordered() | bold_walls() | bg(Color::Light_blue) |
            fg(Color::Blue);
    }

   protected:
    auto mouse_press_event(npp::Mouse const& m) -> bool override
    {
        if (m.button == npp::Mouse::Button::Left)
            points_.push_back(m.local);
        this->update();  // post a paint event to *this
        return Widget::mouse_press_event(m);
    }

    auto paint_event() -> bool override
    {
        auto p = npp::painter{*this};
        for (auto point : points_)
            p.put(L'X', point);
        return Widget::paint_event();
    }

   private:
    std::vector<npp::Point> points_;
};
```

    This Widget will paint an `X` to the screen at every point the
        mouse's left button has been clicked.The paint event always starts with
            a blank slate,
    so a history is held by `points_` and each element is painted to the screen
        on each call to `paint_event`.

    ##Extending Existing Widgets

        The same tools can be used to extend existing Widgets;
by inheriting from the Widget and overriding event handlers,
    or changing traits via the constructor
            .

        The code below will create a new Textbox type that changes its
            background color when it is in focus.

```cpp class Focus_indicator_textbox : public npp::Textbox {
   public:
    Focus_indicator_textbox(Glyph_string text) : Textbox{std::move(text)}
    {
        *this | npp::bg(unfocus_color_);
    }

   protected:
    auto focus_in_event() -> bool override
    {
        *this | npp::bg(focus_color_);
        return Widget::focus_in_event();
    }

    auto focus_out_event() -> bool override
    {
        *this | npp::bg(unfocus_color_);
        return Widget::focus_out_event();
    }

   private:
    constexpr static auto focus_color_   = npp::Color::Red;
    constexpr static auto unfocus_color_ = npp::Color::Blue;
};
```

    ##Next

    - [`main()` Function](main - function.md) -
    [Quick Prototyping](quick - prototyping.md)

        ##Previous

    - [Building with Existing Widgets](building - with - existing - widgets.md)
