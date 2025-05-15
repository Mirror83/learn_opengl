#include <vector>

#include "shader.hpp"
#include "opengl_object.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "window.hpp"

#define INFO_LOG_BUFFER_SIZE 512

struct TextureData {
  uint textureId;
  GLenum textureUnit;
  const char *uniformName; 
};

void initTexure(uint *textureId, const char *imagePath, GLenum imageColourFormat);
void render(GLFWwindow *window, Shader shader, std::vector<OpenGLObject> objects, std::vector<TextureData> textures);

int main()
{
  std::vector rectVertices = {
    // positions       // colours         // texture coords
    0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top right
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
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
  // OpenGLObject triangle = OpenGLObject(triangleVertices, vboConfig);
  OpenGLObject rectangle = OpenGLObject(rectVertices, indices, vboConfig);
  std::vector<OpenGLObject> objects = {
    rectangle
  };
  
  Shader shader = Shader("shaders/shader.vert", "shaders/shader.frag");
  
  uint containerTex, faceTex;
  initTexure(&containerTex, "textures/container.jpg", GL_RGB);
  initTexure(&faceTex, "textures/awesomeface.png", GL_RGBA);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  std::vector<TextureData> textures = {
    {containerTex, GL_TEXTURE0, "texture1"},
    {faceTex, GL_TEXTURE1, "texture2"},
  };

  // We currently only have one shader, so we can set it
  // as the active shader outside the render loop
  shader.use();

  for (int i = 0; i < textures.size(); i++)
  {
    shader.setInt(textures[i].uniformName, i);
  }

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    render(window, shader, objects, textures);
    glfwPollEvents();
  }

  glfwTerminate();
  std::cout << "Program terminated." << std::endl;
  return 0;
}

void render(GLFWwindow *window, Shader shader, std::vector<OpenGLObject> objects, std::vector<TextureData> textures)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  for (int i = 0; i < textures.size(); i++)
  {
    TextureData t = textures[i];
    glActiveTexture(t.textureUnit);
    glBindTexture(GL_TEXTURE_2D, t.textureId);
  }
  
  for (int i = 0; i < objects.size(); i++) {
    OpenGLObject object = objects[i];
    object.draw();
  }
  glfwSwapBuffers(window);
}

void initTexure(uint *textureId, const char *imagePath, GLenum imageColourFormat) {
  int width, height, channels;

  // OpenGL expects the (0,0) coordinate of an image to be on the bottom-left,
  // but usually in images it is on the top left. Flipping the image vertically
  // will make OpenGL render it in the intended orientation.
  stbi_set_flip_vertically_on_load(true);
    
  unsigned char *data = stbi_load(imagePath, &width, &height, &channels, 0);

  if (!data) {
    std::cout << "Failed to load texture at: " << imagePath << std::endl;
    stbi_image_free(data);
    exit(1);
  }

  glGenTextures(1, textureId);
  glBindTexture(GL_TEXTURE_2D, *textureId);
  
  // Set the texture wrapping/filtering options (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imageColourFormat, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
}
