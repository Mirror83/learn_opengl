#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#define INFO_LOG_BUFFER_SIZE 512

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void initWindow(GLFWwindow **window);
void initShaders(unsigned int *shaderProgram);
void initVAO(unsigned int *VAO);
void initVBO(unsigned int *VBO);
void initEBO(unsigned int *VBO);
void render(GLFWwindow *window, unsigned int shaderProgram, unsigned int VAO, unsigned int EBO);

// Unique vertices for a rectangle
float vertices[] = {
    -0.5f, 0.5f, 0.0f, // top left
    -0.5f, -0.5f, 0.0f, // bottom left
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
};

// Order for drawing triangles that correspond to the 
// desired rectangle given the vertices above
unsigned int indices[] = {
    0, 1, 2, // First triangle
    2, 3, 1 // Second triangle
};


const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragShaderSource = "#version 330 core\n"
                               "out vec4 FragColour;\n"
                               "void main()\n"
                               "{\n"
                               "    FragColour = vec4(1.0, 0.5, 0.2, 1.0);\n"
                               "}\0";

int main()
{
    std::cout << "Running program...\n";
    GLFWwindow *window;

    initWindow(&window);

    unsigned int VAO;
    initVAO(&VAO);

    unsigned int VBO;
    initVBO(&VBO);

    unsigned int EBO;
    initEBO(&EBO);

    unsigned int shaderProgram;
    initShaders(&shaderProgram);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        render(window, shaderProgram, VAO, EBO);
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << "Program terminated." << std::endl;
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void initWindow(GLFWwindow **window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); (Uncomment for Mac OS X)
    int windowWidth = 800;
    int windowHeight = 600;
    const char *windowTitle = "Learn OpenGL";

    *window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(*window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
}

void initShaders(unsigned int *shaderProgram)
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[INFO_LOG_BUFFER_SIZE];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, INFO_LOG_BUFFER_SIZE, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        exit(-1);
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, INFO_LOG_BUFFER_SIZE, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        exit(-1);
    }

    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, vertexShader);
    glAttachShader(*shaderProgram, fragmentShader);
    glLinkProgram(*shaderProgram);
    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(*shaderProgram, INFO_LOG_BUFFER_SIZE, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK::LINKING_FAILED\n"
                  << infoLog << std::endl;
        exit(-1);
    }

    glUseProgram(*shaderProgram);
    // Delete shader objects after linking to shader program object
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void initVBO(unsigned int *VBO)
{
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    int location = 0;
    int elementsPerVertex = 3;
    int stride = elementsPerVertex * sizeof(float);
    void *offset = (void *)0;
    glVertexAttribPointer(location, elementsPerVertex, GL_FLOAT, GL_FALSE, stride,
                          offset);
    glEnableVertexAttribArray(location);
}

void initVAO(unsigned int *VAO)
{
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
}

void initEBO(unsigned int *EBO)
{
    glGenBuffers(1, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void render(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO, unsigned int EBO) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(vertices[0]));

    glfwSwapBuffers(window);

}