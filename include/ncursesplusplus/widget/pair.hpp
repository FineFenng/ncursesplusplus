#ifndef NCURSESPLUSPLUS_WIDGET_PAIR_HPP
#define NCURSESPLUSPLUS_WIDGET_PAIR_HPP

namespace npp {

/// Heterogeneous pair of Widgets within a Layout_t.
template<typename Layout_t,
    typename Widget_one_t = void,
    typename Widget_two_t = void>
class Pair : public Layout_t {
 public:
  Widget_one_t &first = this->template MakeChild<Widget_one_t>();
  Widget_two_t &second = this->template MakeChild<Widget_two_t>();

 public:
  using Base = Layout_t;
};

template<typename Layout_t>
class Pair<Layout_t, void, void> : public Layout_t {
 public:
  typename Layout_t::Child_t &first = this->template MakeChild();
  typename Layout_t::Child_t &second = this->template MakeChild();

 public:
  using Base = Layout_t;
};

}  // namespace npp
#endif  // NCURSESPLUSPLUS_WIDGET_PAIR_HPP
