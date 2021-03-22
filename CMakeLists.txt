cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(ncursesplusplus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

# Build Types
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}
    CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel tsan asan lsan msan ubsan"
    FORCE
)

# ThreadSanitizer
set(CMAKE_CXX_FLAGS_TSAN
    "-fsanitize=thread -g -O1"
    CACHE STRING "Flags used by the C++ compiler during ThreadSanitizer builds."
    FORCE
)

# AddressSanitize
set(CMAKE_CXX_FLAGS_ASAN
    "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1"
    CACHE STRING "Flags used by the C++ compiler during AddressSanitizer builds."
    FORCE
)

# LeakSanitizer
set(CMAKE_CXX_FLAGS_LSAN
    "-fsanitize=leak -fno-omit-frame-pointer -g -O1"
    CACHE STRING "Flags used by the C++ compiler during LeakSanitizer builds."
    FORCE
)

# MemorySanitizer
set(CMAKE_CXX_FLAGS_MSAN
    "-fsanitize=memory -fno-optimize-sibling-calls -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer -g -O2"
    CACHE STRING "Flags used by the C++ compiler during MemorySanitizer builds."
    FORCE
)

# UndefinedBehaviour
set(CMAKE_CXX_FLAGS_UBSAN
    "-fsanitize=undefined"
    CACHE STRING "Flags used by the C++ compiler during UndefinedBehaviourSanitizer builds."
    FORCE
)

message("Build Type: ${CMAKE_BUILD_TYPE}" )

add_subdirectory(3rd)

if(${CMAKE_VERSION} VERSION_LESS "3.8")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(NPP_TARGET_NAME ${PROJECT_NAME})

set(${NPP_TARGET_NAME}_COMMON_HDR
        include/ncursesplusplus/common/filter_iterator.hpp
        include/ncursesplusplus/common/identity.hpp
        include/ncursesplusplus/common/lockable.hpp
        include/ncursesplusplus/common/overload.hpp
        include/ncursesplusplus/common/priority_queue.hpp
        include/ncursesplusplus/common/range.hpp
        include/ncursesplusplus/common/transform_iterator.hpp
        include/ncursesplusplus/common/transform_view.hpp
        include/ncursesplusplus/common/meta.hpp)

set(${NPP_TARGET_NAME}_PAINTER_HDR
        include/ncursesplusplus/painter/brush.hpp
        include/ncursesplusplus/painter/color.hpp
        include/ncursesplusplus/painter/detail/find_empty_space.hpp
        include/ncursesplusplus/painter/detail/is_paintable.hpp
        include/ncursesplusplus/painter/detail/screen.hpp
        include/ncursesplusplus/painter/detail/screen_descriptor.hpp
        include/ncursesplusplus/painter/detail/screen_mask.hpp
        include/ncursesplusplus/painter/detail/staged_changes.hpp
        include/ncursesplusplus/painter/dynamic_colors.hpp
        include/ncursesplusplus/painter/glyph.hpp
        include/ncursesplusplus/painter/glyph_matrix.hpp
        include/ncursesplusplus/painter/glyph_string.hpp
        include/ncursesplusplus/painter/painter.hpp
        include/ncursesplusplus/painter/palette/amstrad_cpc.hpp
        include/ncursesplusplus/painter/palette/apple_ii.hpp
        include/ncursesplusplus/painter/palette/ashes.hpp
        include/ncursesplusplus/painter/palette/basic.hpp
        include/ncursesplusplus/painter/palette/basic8.hpp
        include/ncursesplusplus/painter/palette/cga.hpp
        include/ncursesplusplus/painter/palette/commodore_64.hpp
        include/ncursesplusplus/painter/palette/commodore_vic20.hpp
        include/ncursesplusplus/painter/palette/dawn_bringer16.hpp
        include/ncursesplusplus/painter/palette/dawn_bringer32.hpp
        include/ncursesplusplus/painter/palette/en4.hpp
        include/ncursesplusplus/painter/palette/gameboy.hpp
        include/ncursesplusplus/painter/palette/gameboy_pocket.hpp
        include/ncursesplusplus/painter/palette/macintosh_ii.hpp
        include/ncursesplusplus/painter/palette/msx.hpp
        include/ncursesplusplus/painter/palette/nes.hpp
        include/ncursesplusplus/painter/palette/savanna.hpp
        include/ncursesplusplus/painter/palette/secam.hpp
        include/ncursesplusplus/painter/palette/stormy6.hpp
        include/ncursesplusplus/painter/palette/teletext.hpp
        include/ncursesplusplus/painter/palette/thomson_m05.hpp
        include/ncursesplusplus/painter/palette/windows.hpp
        include/ncursesplusplus/painter/palette/windows_console.hpp
        include/ncursesplusplus/painter/palette/zx_spectrum.hpp
        include/ncursesplusplus/painter/trait.hpp
        include/ncursesplusplus/painter/utility/wchar_to_bytes.hpp
        )

set(${NPP_TARGET_NAME}_SYSTEM_HDR
        include/ncursesplusplus/system/animation_engine.hpp
        include/ncursesplusplus/system/detail/event_engine.hpp
        include/ncursesplusplus/system/detail/event_print.hpp
        include/ncursesplusplus/system/detail/event_queue.hpp
        include/ncursesplusplus/system/detail/filter_send.hpp
        include/ncursesplusplus/system/detail/find_widget_at.hpp
        include/ncursesplusplus/system/detail/focus.hpp
        include/ncursesplusplus/system/detail/interval_event_loop.hpp
        include/ncursesplusplus/system/detail/is_sendable.hpp
        include/ncursesplusplus/system/detail/send.hpp
        include/ncursesplusplus/system/detail/timer_event_loop.hpp
        include/ncursesplusplus/system/detail/user_input_event_loop.hpp
        include/ncursesplusplus/system/event.hpp
        include/ncursesplusplus/system/event_fwd.hpp
        include/ncursesplusplus/system/event_loop.hpp
        include/ncursesplusplus/system/event_name.hpp
        include/ncursesplusplus/system/key.hpp
        include/ncursesplusplus/system/mouse.hpp
        include/ncursesplusplus/system/shortcuts.hpp
        include/ncursesplusplus/system/system.hpp
        )

set(${NPP_TARGET_NAME}_TERMINAL_HDR
        include/ncursesplusplus/terminal/dynamic_color_engine.hpp
        include/ncursesplusplus/terminal/input.hpp
        include/ncursesplusplus/terminal/output.hpp
        include/ncursesplusplus/terminal/terminal.hpp
        include/ncursesplusplus/terminal/terminal_error.hpp
        )

set(${NPP_TARGET_NAME}_WIDGET_HDR
        include/ncursesplusplus/widget/align.hpp
        include/ncursesplusplus/widget/area.hpp
        include/ncursesplusplus/widget/array.hpp
        include/ncursesplusplus/widget/border.hpp
        include/ncursesplusplus/widget/cursor.hpp
        include/ncursesplusplus/widget/detail/border_offset.hpp
        include/ncursesplusplus/widget/detail/graph_tree.hpp
        include/ncursesplusplus/widget/detail/link_lifetimes.hpp
        include/ncursesplusplus/widget/detail/memfn_signature.hpp
        include/ncursesplusplus/widget/focus_policy.hpp
        include/ncursesplusplus/widget/growth.hpp
        include/ncursesplusplus/widget/layout.hpp
        include/ncursesplusplus/widget/layouts/detail/layout_span.hpp
        include/ncursesplusplus/widget/layouts/detail/linear_layout.hpp
        include/ncursesplusplus/widget/layouts/detail/shared_space.hpp
        include/ncursesplusplus/widget/layouts/detail/unique_space.hpp
        include/ncursesplusplus/widget/layouts/fixed.hpp
        include/ncursesplusplus/widget/layouts/float.hpp
        include/ncursesplusplus/widget/layouts/horizontal.hpp
        include/ncursesplusplus/widget/layouts/opposite.hpp
        include/ncursesplusplus/widget/layouts/passive.hpp
        include/ncursesplusplus/widget/layouts/selecting.hpp
        include/ncursesplusplus/widget/layouts/set.hpp
        include/ncursesplusplus/widget/layouts/stack.hpp
        include/ncursesplusplus/widget/layouts/vertical.hpp
        include/ncursesplusplus/widget/pair.hpp
        include/ncursesplusplus/widget/pipe.hpp
        include/ncursesplusplus/widget/point.hpp
        include/ncursesplusplus/widget/size_policy.hpp
        include/ncursesplusplus/widget/tuple.hpp
        include/ncursesplusplus/widget/widget.hpp
        include/ncursesplusplus/widget/widget_slots.hpp
        include/ncursesplusplus/widget/widgets/accordion.hpp
        include/ncursesplusplus/widget/widgets/banner.hpp
        include/ncursesplusplus/widget/widgets/button.hpp
        include/ncursesplusplus/widget/widgets/button_list.hpp
        include/ncursesplusplus/widget/widgets/checkbox.hpp
        include/ncursesplusplus/widget/widgets/color_select.hpp
        include/ncursesplusplus/widget/widgets/confirm_button.hpp
        include/ncursesplusplus/widget/widgets/cycle_box.hpp
        include/ncursesplusplus/widget/widgets/cycle_stack.hpp
        include/ncursesplusplus/widget/widgets/detail/nearly_equal.hpp
        include/ncursesplusplus/widget/widgets/detail/slider_logic.hpp
        include/ncursesplusplus/widget/widgets/detail/textbox_base.hpp
        include/ncursesplusplus/widget/widgets/graph.hpp
        include/ncursesplusplus/widget/widgets/hideable.hpp
        include/ncursesplusplus/widget/widgets/horizontal_slider.hpp
        include/ncursesplusplus/widget/widgets/label.hpp
        include/ncursesplusplus/widget/widgets/line_edit.hpp
        include/ncursesplusplus/widget/widgets/log.hpp
        include/ncursesplusplus/widget/widgets/matrix_display.hpp
        include/ncursesplusplus/widget/widgets/menu.hpp
        include/ncursesplusplus/widget/widgets/menu_stack.hpp
        include/ncursesplusplus/widget/widgets/notify_light.hpp
        include/ncursesplusplus/widget/widgets/number_edit.hpp
        include/ncursesplusplus/widget/widgets/open_file.hpp
        include/ncursesplusplus/widget/widgets/save_file.hpp
        include/ncursesplusplus/widget/widgets/scrollbar.hpp
        include/ncursesplusplus/widget/widgets/selectable.hpp
        include/ncursesplusplus/widget/widgets/spinner.hpp
        include/ncursesplusplus/widget/widgets/status_bar.hpp
        include/ncursesplusplus/widget/widgets/text_display.hpp
        include/ncursesplusplus/widget/widgets/textbox.hpp
        include/ncursesplusplus/widget/widgets/tile.hpp
        include/ncursesplusplus/widget/widgets/titlebar.hpp
        include/ncursesplusplus/widget/widgets/toggle_button.hpp
        include/ncursesplusplus/widget/widgets/vertical_slider.hpp
        )

set(${NPP_TARGET_NAME}_HDR
        ${${NPP_TARGET_NAME}_COMMON_HDR}
        ${${NPP_TARGET_NAME}_PAINTER_HDR}
        ${${NPP_TARGET_NAME}_SYSTEM_HDR}
        ${${NPP_TARGET_NAME}_TERMINAL_HDR}
        ${${NPP_TARGET_NAME}_WIDGET_HDR}
        )

set(${NPP_TARGET_NAME}_SRC
        src/painter/find_empty_space.cpp
        src/painter/glyph_matrix.cpp
        src/painter/painter.cpp
        src/painter/screen.cpp
        src/painter/screen_mask.cpp
        src/system/animation_engine.cpp
        src/system/event_loop.cpp
        src/system/find_widget_at.cpp
        src/system/focus.cpp
        src/system/system.cpp
        src/system/timer_event_loop.cpp
        src/system/user_input_event_loop.cpp
        src/terminal/dynamic_color_engine.cpp
        src/terminal/input.cpp
        src/terminal/output.cpp
        src/terminal/terminal.cpp
        src/widget/border.cpp
        src/widget/border_offset.cpp
        src/widget/cycle_box.cpp
        src/widget/graph_tree.cpp
        src/widget/horizontal_slider.cpp
        src/widget/line_edit.cpp
        src/widget/log.cpp
        src/widget/menu.cpp
        src/widget/text_display.cpp
        src/widget/textbox.cpp
        src/widget/textbox_base.cpp
        src/widget/vertical_slider.cpp
        src/widget/widget.cpp
        src/widget/widget_slots.cpp
        )


add_library(${NPP_TARGET_NAME} SHARED
        ${${NPP_TARGET_NAME}_HDR}
        ${${NPP_TARGET_NAME}_SRC}
        )

target_link_libraries(${NPP_TARGET_NAME} PRIVATE
        absl_optional
        absl_synchronization
        absl_type_traits
        absl_variant
        )

target_include_directories(${NPP_TARGET_NAME}
        PUBLIC include/ncursesplusplus/ncursesplusplus.h
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
        )


# Add Demos
add_subdirectory(demos)

# Add Tests
add_subdirectory(tests)
