cmake_minimum_required(VERSION 3.15)

project(clc. VERSION 1.0
                  DESCRIPTION "super simple stopwatch for debian"
                  LANGUAGES CXX)

add_executable(clc src/clc.cpp)
target_compile_options(clc PRIVATE
    -Wall
    -Wextra
    -O2
)
target_link_libraries(clc PRIVATE
  X11
  GL
  Xrandr
)
target_include_directories(clc PRIVATE ${XRANDR_INCLUDE_DIRS})
