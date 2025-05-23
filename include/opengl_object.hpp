#pragma once
#include <vector>

#include "ext/glad/glad.h"

typedef unsigned int uint;

struct VBOConfig
{
  uint location, elementsPerItem;
};

typedef std::vector<VBOConfig> VBOConfigList;

class OpenGLObject
{
  std::vector<float> _vertices;
  std::vector<uint> _indices;
  // Vertex Array, Vertex and Element Buffer object IDs
  unsigned int VAO, VBO, EBO, stride;

  void _initVAO()
  {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
  }

  void _initVBO(const VBOConfigList& config)
  {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

    stride = 0;
    for (int i = 0; i < config.size(); i++)
    {
      stride += config[i].elementsPerItem;
    }

    stride *= sizeof(float);

    for (int i = 0; i < config.size(); i++)
    {
      const auto [location, elementsPerItem] = config[i];
      void *offset;
      if (location == 0){
        offset = reinterpret_cast<void*>(location);
      } else {
        const uint elementsInPrevItem = config[i-1].elementsPerItem;
        offset = reinterpret_cast<void*>(elementsInPrevItem * location * sizeof(float));
      }
      glVertexAttribPointer(location, elementsPerItem, GL_FLOAT, GL_FALSE, stride, offset);
      glEnableVertexAttribArray(location);
    }
  }

  void _initEBO() {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint), _indices.data(), GL_STATIC_DRAW);
  }

public:
  OpenGLObject(const std::vector<float>& vertices, const VBOConfigList& vboConfig)
  {
    _vertices = vertices;
    _initVAO();
    _initVBO(vboConfig);
  }

  OpenGLObject(const std::vector<float>& vertices, const std::vector<uint>& indices, const VBOConfigList& vboConfig)
  {
    _vertices = vertices;
    _indices = indices;
    _initVAO();
    _initVBO(vboConfig);
    _initEBO();
  }

  void draw() const
  {
    glBindVertexArray(VAO);
    if (_indices.size() > 0) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
    } else {
      // Stores the points per triangle + texture coordinates and other vertex shader input params
      const int elementsPerVertex = _vertices.size() / (stride / sizeof(float));
      glDrawArrays(GL_TRIANGLES, 0, elementsPerVertex);
    }
  }
};