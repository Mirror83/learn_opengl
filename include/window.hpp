#pragma once

#include <iostream>

#include "ext/glad/glad.h"
#include "GLFW/glfw3.h"


constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

inline void framebuffer_size_callback(GLFWwindow *, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

inline void initWindow(GLFWwindow **window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); (Uncomment for Mac OS X)
    const char *windowTitle = "Learn OpenGL";

    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowTitle, nullptr, nullptr);
    if (*window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(*window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
}