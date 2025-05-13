#include "shader.hpp"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
#include <math.h>

#define INFO_LOG_BUFFER_SIZE 512

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void initWindow(GLFWwindow **window);
void initVAO(unsigned int *VAO);
void initVBO(unsigned int *VBO, std::vector<float> vertices);
void initEBO(unsigned int *EBO, std::vector<unsigned int> indices);
void initTexure(unsigned int *textureId, const char *imagePath);
void render(GLFWwindow *window, Shader shader, unsigned int rectangleVAO, unsigned int triangleVAO, unsigned int rectangleEBO, std::vector<unsigned int> indices, unsigned int texture);
void drawTriangle(unsigned int triangleVAO);
void drawRectangle(unsigned int rectangleVAO, unsigned int rectangleEBO, std::vector<unsigned int> indices);

int main()
{
  std::vector vertices = {
    // positions       // colours         // texture coords
    0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top right
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// bottom left
   -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top left
  };

  std::vector<unsigned int> indices = {
    0, 1, 2, // First triangle
    2, 3, 0,  // Second triangle
  };

  std::vector triangleVertices = {
    // positions       // colours        // texture coords
    0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // left
    0.2f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,// top
    0.4f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // right
  };

  std::cout << "Running program...\n";
  GLFWwindow *window;

  initWindow(&window);

  unsigned int rectangleVAO, triangleVAO;
  unsigned int rectangleVBO, triangleVBO;
  unsigned int rectangleEBO;
  unsigned int texture;

  initVAO(&rectangleVAO);
  initVBO(&rectangleVBO, vertices);
  initEBO(&rectangleEBO, indices);

  initVAO(&triangleVAO);
  initVBO(&triangleVBO, triangleVertices);

  initTexure(&texture, "textures/container.jpg");

  Shader shader = Shader("shaders/shader.vert", "shaders/shader.frag");
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    render(window, shader, rectangleVAO, triangleVAO, rectangleEBO, indices, texture);
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

  int stride = 8 * sizeof(float);
  int elementsPerItem = 3;

  int positionLocation = 0;
  int posOffset = elementsPerItem * positionLocation * sizeof(float);
  glVertexAttribPointer(positionLocation, elementsPerItem, GL_FLOAT, GL_FALSE, stride,
                        (void *)0);
  glEnableVertexAttribArray(positionLocation);

  int colourLocation = 1;
  int colourOffset = elementsPerItem * colourLocation * sizeof(float);
  glVertexAttribPointer(colourLocation, elementsPerItem, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(colourLocation);

  int texCoordsLocation = 2;
  int texCoordsOffset = elementsPerItem * texCoordsLocation * sizeof(float);
  glVertexAttribPointer(texCoordsLocation, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(texCoordsLocation);
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

void initTexure(unsigned int *textureId, const char *imagePath) {
  int width, height, channels;

  unsigned char *data = stbi_load(imagePath, &width, &height, &channels, 0);
  glGenTextures(1, textureId);
  glBindTexture(GL_TEXTURE_2D, *textureId);
  
  // Set the texture wrapping/filtering options (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
  } else {
    std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);
    exit(1);
  }
}

void render(GLFWwindow *window, Shader shader, unsigned int rectangleVAO, unsigned int triangleVAO, unsigned int rectangleEBO, std::vector<unsigned int> indices, unsigned int texture)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader.use();
  glBindTexture(GL_TEXTURE_2D, texture);
  drawRectangle(rectangleVAO, rectangleEBO, indices);
  // drawTriangle(triangleVAO);

  glfwSwapBuffers(window);
}

void drawTriangle(unsigned int triangleVAO) {
  glBindVertexArray(triangleVAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void drawRectangle(unsigned int rectangleVAO, unsigned int rectangleEBO, std::vector<unsigned int> indices) {
  glBindVertexArray(rectangleVAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangleEBO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}