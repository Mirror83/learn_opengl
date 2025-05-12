#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <vector>

#define INFO_LOG_BUFFER_SIZE 512

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void initWindow(GLFWwindow **window);
void initVAO(unsigned int *VAO);
void initVBO(unsigned int *VBO, std::vector<float> vertices);
void initEBO(unsigned int *EBO, std::vector<unsigned int> indices);
void render(GLFWwindow *window, Shader shader, unsigned int VAO, unsigned int EBO, std::vector<unsigned int> indices);

int main()
{
  std::vector vertices = {
    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top left
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom right
  };

  std::vector<unsigned int> indices = {
    0, 1, 2, // First triangle
    2, 3, 1  // Second triangle
  };

  std::cout << "Running program...\n";
  GLFWwindow *window;

  initWindow(&window);

  unsigned int VAO;
  initVAO(&VAO);

  unsigned int VBO;
  initVBO(&VBO, vertices);

  unsigned int EBO;
  initEBO(&EBO, indices);

  Shader shader = Shader("shaders/shader.vert", "shaders/shader.frag");
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    render(window, shader, VAO, EBO, indices);
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

void initVBO(unsigned int *VBO, std::vector<float> vertices)
{
  glGenBuffers(1, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  int stride = 6 * sizeof(float);
  int elementsPerItem = 3;

  int positionLocation = 0;
  glVertexAttribPointer(positionLocation, elementsPerItem, GL_FLOAT, GL_FALSE, stride,
                        (void *)0);
  glEnableVertexAttribArray(positionLocation);

  int colourLocation = 1;
  glVertexAttribPointer(colourLocation, elementsPerItem, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(colourLocation);
}

void initVAO(unsigned int *VAO)
{
  glGenVertexArrays(1, VAO);
  glBindVertexArray(*VAO);
}

void initEBO(unsigned int *EBO, std::vector<unsigned int> indices)
{
  glGenBuffers(1, EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void render(GLFWwindow *window, Shader shader, unsigned int VAO, unsigned int EBO, std::vector<unsigned int> indices)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader.use();

  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glDrawElements(GL_TRIANGLES, indices.size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
  // glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(vertices[0]));

  glfwSwapBuffers(window);
}