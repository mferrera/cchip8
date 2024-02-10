add_library(cchip8
    audio.cpp
    cpu.cpp
    display.cpp
    emulator.cpp
    input.cpp
    memory.cpp
    menu.cpp
    rom.cpp
    window.cpp)

set_target_properties(cchip8 PROPERTIES
    PUBLIC_HEADER
        "${PROJECT_SOURCE_DIR}/include/cchip8/rom.h"
    PUBLIC_HEADER
        "${PROJECT_SOURCE_DIR}/include/cchip8/emulator.h")
target_include_directories(cchip8
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_link_libraries(cchip8 PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)
