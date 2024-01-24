add_executable(cchip8
    cli/chip8.cpp
    cchip8/audio.cpp
    cchip8/cpu.cpp
    cchip8/display.cpp
    cchip8/emulator.cpp
    cchip8/input.cpp
    cchip8/memory.cpp
    cchip8/rom.cpp)
set_target_properties(
    cchip8 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
target_include_directories(cchip8 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(cchip8 PRIVATE SDL3::SDL3)
