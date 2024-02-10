add_executable(chip8 cchip8.cpp)
target_link_libraries(chip8 PRIVATE cchip8 SDL3::SDL3 SDL3_ttf::SDL3_ttf)
set_target_properties(
    chip8 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
