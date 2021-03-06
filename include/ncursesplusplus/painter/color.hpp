#ifndef NCURSESPLUSPLUS_PAINTER_COLOR_HPP
#define NCURSESPLUSPLUS_PAINTER_COLOR_HPP

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "absl/types/variant.h"

namespace npp {

/// Color numbers [0 - 180] are valid.
class Color {
 public:
  using Value = std::uint8_t;
  Value value;

 public:
  enum Name : Value {
    Background = 0,  // Default Background; should you change the name?
    Black = 0,
    Dark_red = 1,
    Green = 2,
    Brown = 3,
    Dark_blue = 4,
    Violet = 5,
    Light_blue = 6,
    Light_gray = 7,
    Dark_gray = 8,
    Red = 9,
    Light_green = 10,
    Yellow = 11,
    Blue = 12,
    Orange = 13,
    Gray = 14,
    White = 15,
    Foreground = 15,  // Default Foreground
  };

 public:
  constexpr Color(Name n) : value{static_cast<Value>(n)} {}
  constexpr explicit Color(Value v) : value{v} {}
};

constexpr bool operator==(Color x, Color y) {
  return x.value == y.value;
}

constexpr bool operator!=(Color x, Color y) {
  return !(x == y);
}

struct Background_color {
  Color::Value value;
};

struct Foreground_color {
  Color::Value value;
};

/// Converts a Color into a Background_color to be used by Brush.
constexpr Background_color bg(Color c) {
  return {c.value};
}

/// Converts a Color into a Background_color to be used by Brush.
constexpr Foreground_color fg(Color c) {
  return {c.value};
}

/* --------------------------- ANSI/XTERM Color ------------------------------*/

/// ANSI color numbers [0 - 255].
struct ANSI {
  using Value_t = std::uint8_t;
  Value_t value;
};

inline bool operator==(ANSI a, ANSI b) {
  return a.value == b.value;
}

inline bool operator!=(ANSI a, ANSI b) {
  return a.value != b.value;
}

/* ------------------------------- True Color --------------------------------*/

/// Holds Red, Green, and Blue values, valid range of [0-255].
struct RGB {
  using Value = std::uint8_t;
  Value red;
  Value green;
  Value blue;
};

/// Holds Hue, Saturation, Lightness values of a color.
struct HSL {
  std::uint16_t hue;        // [0, 359] degrees
  std::uint8_t saturation;  // [0, 100] %
  std::uint8_t lightness;   // [0, 100] %
};

/// Holds a True Color definition in terms of RGB values.
class True_color {
 public:
  using Value_t = std::uint32_t;

 public:
  constexpr explicit True_color(Value_t hex) : value_{hex} {}

  constexpr explicit True_color(RGB x) : value_{merge(x)} {}

  constexpr explicit True_color(HSL x) : value_{merge(hsl_to_rgb(x))} {}

 public:
  /// Returns the red component value [0, 255]
  constexpr std::uint8_t red() const {
    return (value_ >> 16) & 0xFF;
  }

  /// Returns the green component value [0, 255]
  constexpr std::uint8_t green() const {
    return (value_ >> 8) & 0xFF;
  }

  /// Returns the blue component value [0, 255]
  constexpr std::uint8_t blue() const {
    return value_ & 0xFF;
  }

 private:
  Value_t value_;

 private:
  /// Merge RGB values into single 24 bit value
  static constexpr Value_t merge(RGB x) {
    return (x.red << 16) + (x.green << 8) + x.blue;
  }

  template<typename T>
  static constexpr Value_t abs(T x) {
    return (x == T(0) ? T(0) : x < T(0) ? -x : x);
  }

  template<typename T>
  static constexpr Value_t fmod(const T x, const T y) {
    return (x - T(static_cast<long long>(x / y)) * y);
  }

  static constexpr RGB hsl_to_rgb(HSL v) {
    double const lightness = v.lightness / 100.;
    double const saturation = v.saturation / 100.;

    auto const c = (1 - abs((2 * lightness) - 1.)) * saturation;
    double const h_prime = v.hue / 60.;
    double const x = c * (1. - abs(fmod(h_prime, 2.) - 1.));
    double const m = lightness - (c / 2.);

    auto const c_ = static_cast<RGB::Value>((c + m) * 255);
    auto const x_ = static_cast<RGB::Value>((x + m) * 255);
    auto const m_ = static_cast<RGB::Value>(m * 255);

    if (v.hue < 60.)
      return {c_, x_, m_};
    if (v.hue < 120.)
      return {x_, c_, m_};
    if (v.hue < 180.)
      return {m_, c_, x_};
    if (v.hue < 240.)
      return {m_, x_, c_};
    if (v.hue < 300.)
      return {x_, m_, c_};
    if (v.hue < 360.)
      return {c_, m_, x_};
    else
      return {0, 0, 0};
  }

 public:
  static /* constexpr */ HSL rgb_to_hsl(RGB x) {
    double const r_prime = x.red / 255.;
    double const g_prime = x.green / 255.;
    double const b_prime = x.blue / 255.;

    double const c_max = std::max({r_prime, g_prime, b_prime});
    double const c_min = std::min({r_prime, g_prime, b_prime});
    double const delta = c_max - c_min;

    double const lightness = (c_max + c_min) / 2.;
    double const saturation = [&] {
      if (delta == 0.)
        return 0.;
      double const den = 1. - abs(2. * lightness - 1.);
      return delta / den;
    }();
    std::uint16_t const hue = [&] {
      if (delta == 0.)
        return 0.;
      if (c_max == r_prime)
        return 60. * fmod((g_prime - b_prime) / delta, 6.);
      if (c_max == g_prime)
        return 60. * (((b_prime - r_prime) / delta) + 2.);
      if (c_max == b_prime)
        return 60. * (((r_prime - g_prime) / delta) + 4.);
      return .0;
    }();
    return {static_cast<std::uint16_t>(hue),
            static_cast<std::uint8_t>(saturation * 100),
            static_cast<std::uint8_t>(lightness * 100)};
  }
};

/// Returns a True_color from RGB values. Convinience for defining palettes.
inline constexpr True_color rgb(RGB::Value r, RGB::Value g, RGB::Value b) {
  return static_cast<True_color>(RGB{r, g, b});
}

/// Returns a True_color from HSL values. Convinience for defining palettes.
inline constexpr True_color hsl(std::uint16_t h, std::uint8_t s, std::uint8_t l) {
  return static_cast<True_color>(HSL{h, s, l});
}

/* ----------------------------- Dynamic Color -------------------------------*/

/// Defines an animated color
/** get_value() is called every interval and used to update the color. */
struct Dynamic_color {
  using Period_t = std::chrono::milliseconds;
  Period_t interval;
  std::function<True_color()> get_value;
};

/* ----------------------------- Color Palette -------------------------------*/

/// Used to define a single color for a Palette.
class Color_definition {
 public:
  using Value_t = absl::variant<absl::monostate, True_color, Dynamic_color>;

 public:
  Value_t value;
  Color color;
  ANSI ansi;

 public:
  Color_definition(Color c, ANSI a)
      : value{absl::monostate{}}, color{c}, ansi{a} {}

  Color_definition(Color c, ANSI a, Value_t v)
      : value{std::move(v)}, color{c}, ansi{a} {}
};

/// Max size of 181 colors in a palette
/** 181 colors will need 32,761 pairs, max_pairs in ncurses is 32,767. */
using Palette = std::vector<Color_definition>;

/// Create a Palette by pairing Color_definition::Value with Colors.
/** Colors are paired in order Color_definition::Value's are added,
 *  starting with Color{0}. */
template<typename... ColorValues>
static auto make_palette(ColorValues... values) -> Palette {
  auto c = Color::Value{0};
  auto a = ANSI::Value_t{16};
  return {Color_definition{Color{c++}, ANSI{a++}, values}...};
}

/* ---------------------------------------------------------------------------*/

/// Get the default name of the color as a string.
inline auto color_to_string(Color c) -> std::string {
  switch (c.value) {
    case 0: return "Black";
    case 1: return "Dark Red";
    case 2: return "Green";
    case 3: return "Brown";
    case 4: return "Dark Blue";
    case 5: return "Violet";
    case 6: return "Light Blue";
    case 7: return "Light Gray";
    case 8: return "Dark Gray";
    case 9: return "Red";
    case 10: return "Light Green";
    case 11: return "Yellow";
    case 12: return "Blue";
    case 13: return "Orange";
    case 14: return "Gray";
    case 15: return "White Gray";
    default: return "";
  }
}

}  // namespace npp
#endif  // NCURSESPLUSPLUS_PAINTER_COLOR_HPP
