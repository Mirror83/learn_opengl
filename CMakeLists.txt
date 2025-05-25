cmake_minimum_required(VERSION 3.31)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 20)

add_executable(getting_started src/getting-started/main.cpp src/ext/glad.c)
target_include_directories(getting_started PUBLIC include)
target_link_libraries(getting_started glfw pthread wayland-client wayland-cursor wayland-egl xkbcommon)
