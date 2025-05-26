cmake_minimum_required(VERSION 3.31)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 20)

set(OPEN_GL_LIBRARIES glfw pthread)
set(WAYLAND_LIBRARIES wayland-client wayland-cursor wayland-egl xkbcommon)
set(GLAD "src/ext/glad.c")

include_directories(include)
link_libraries(${OPEN_GL_LIBRARIES} ${WAYLAND_LIBRARIES})

add_executable(getting_started src/getting-started/main.cpp ${GLAD})
add_executable(lighting src/lighting/lighting.cpp ${GLAD}
        src/lighting/lighting.hpp)
