#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/fwd.hpp"

class Shader
{
public:
  // Shader program ID
  unsigned int programId;

  Shader(const char *vertexPath, const char *fragmentPath)
  {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);

      std::stringstream vShaderStream, fShaderStream;
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      vShaderFile.close();
      fShaderFile.close();

      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
      exit(-1);
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertexId, fragmentId;
    int success;
    char infoLog[512];

    vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &vShaderCode, NULL);
    glCompileShader(vertexId);
    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << infoLog << std::endl;
      exit(-1);
    }

    fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &fShaderCode, NULL);
    glCompileShader(fragmentId);
    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                << infoLog << std::endl;
      exit(-1);
    }

    programId = glCreateProgram();
    glAttachShader(programId, vertexId);
    glAttachShader(programId, fragmentId);
    glLinkProgram(programId);

    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(programId, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
      exit(-1);
    }

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
  }

  void use()
  {
    glUseProgram(programId);
  }

  void setBool(const std::string &name, bool value) const
  {
    glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
  }

  void setInt(const std::string &name, int value) const
  {
    glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
  }

  void setFloat(const std::string &name, float value) const
  {
    glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
  }

  void set4x4Matrix(const std::string &name, const GLfloat* matrix) const
  {
    const auto loc = glGetUniformLocation(programId, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
  }
};
