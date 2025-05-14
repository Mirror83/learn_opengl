#include "shader.hpp"
#include "opengl_object.hpp"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>

#define INFO_LOG_BUFFER_SIZE 512

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void initWindow(GLFWwindow **window);
void initTexure(unsigned int *textureId, const char *imagePath);
void render(GLFWwindow *window, Shader shader, std::vector<OpenGLObject> objects, unsigned int texture);

int main()
{
  std::vector rectVertices = {
    // positions       // colours         // texture coords
    0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top right
    0.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// bottom left
   -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top left
  };

  std::vector<uint> indices = {
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

  std::vector<VBOConfig> vboConfig = {
    {location: 0, elementsPerItem: 3}, // Position
    {location: 1, elementsPerItem: 3}, // Colour
    {location: 2, elementsPerItem: 2}, // 2D Texture Coords
  };
  OpenGLObject triangle = OpenGLObject(triangleVertices, vboConfig);
  OpenGLObject rectangle = OpenGLObject(rectVertices, indices, vboConfig);
  std::vector<OpenGLObject> objects = {
    triangle,
    rectangle
  };
  
  Shader shader = Shader("shaders/shader.vert", "shaders/shader.frag");
  
  unsigned int texture;
  initTexure(&texture, "textures/container.jpg");
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    render(window, shader, objects, texture);
    glfwPollEvents();
  }

  glfwTerminate();
  std::cout << "Program terminated." << std::endl;
  return 0;
}

void render(GLFWwindow *window, Shader shader, std::vector<OpenGLObject> objects, unsigned int texture)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader.use();
  glBindTexture(GL_TEXTURE_2D, texture);
  for (int i = 0; i < objects.size(); i++) {
    OpenGLObject object = objects[i];
    object.draw();
  }
  glfwSwapBuffers(window);
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
