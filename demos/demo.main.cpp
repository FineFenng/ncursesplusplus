#include "ncursesplusplus/system/system.hpp"

#include "demo_menu.hpp"

int main() { return npp::System{}.run<demos::Demos>(); }

// #include <ncursesplusplus/ncursesplusplus.hpp"

// int main()
// {
//     using namespace npp;
//     auto app_ptr =
//         layout::vertical(
//             textbox(std::to_string(sizeof(Widget)) | fg(Color::Red) |
//             Trait::Bold) | pipe::name("tb"), layout::horizontal<Button>(
//                 button("Button 1") | bg(Color::Blue) |
//                 pipe::name("b1"), button("Button 2") |
//                 bg(Color::Violet) | pipe::name("b2")
//             ) | pipe::fixed_height(1)
//         );
//     // auto& tb = app_ptr->find_child_by_name("tb");
//     // app_ptr->find_child_by_name("b1") | pipe::on_press([&tb]{
//     // tb.append("Button 1 Pressed"); }); app_ptr->find_child_by_name("b2") |
//     // pipe::on_press([&tb]{ tb.append("Button 2 Pressed"); }); return
//     System{}.run(*app_ptr);
// }
