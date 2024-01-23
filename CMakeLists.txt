cmake_minimum_required(VERSION 3.24)

project(cchip8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(cchip8_VERSION_MAJOR   "0")
set(cchip8_VERSION_MINOR   "0")
set(cchip8_VERSION_PATCH   "1")
set(cchip8_VERSION
  "${cchip8_VERSION_MAJOR}.${cchip8_VERSION_MINOR}.${cchip8_VERSION_PATCH}")

option(ENABLE_TESTING "Enable testing and building the tests." OFF)

if (MSVC)
  add_compile_options(/W3 /WX)
else()
  add_compile_options(-W -Wall -Werror)
endif()

include_directories(
  "${PROJECT_BINARY_DIR}/include"
  "."
)
add_subdirectory(src/cchip8)

if(ENABLE_TESTING)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

