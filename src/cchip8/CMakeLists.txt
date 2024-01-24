add_executable(cchip8 main.cpp)
set_target_properties(
    cchip8 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
target_link_libraries(cchip8 PRIVATE SDL3::SDL3)
