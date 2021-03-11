cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ncursesplusplus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

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
        include/ncursesplusplus/common/transform_view.hpp)

set(${NPP_TARGET_NAME}_PAINTER_HDR
        include/ncursesplusplus/painter/.hpp
        include/ncursesplusplus/common/identity.hpp
        include/ncursesplusplus/common/lockable.hpp
        include/ncursesplusplus/common/overload.hpp
        include/ncursesplusplus/common/priority_queue.hpp
        include/ncursesplusplus/common/range.hpp
        include/ncursesplusplus/common/transform_iterator.hpp
        include/ncursesplusplus/common/transform_view.hpp)







# Add nCurses
add_subdirectory(external/ncurses-snapshots)

# Add Signals-Light
add_subdirectory(external/signals-light)

# Add TermOx
add_subdirectory(src)

# Add Demos
add_subdirectory(demos)

# Add Tests
add_subdirectory(tests)
