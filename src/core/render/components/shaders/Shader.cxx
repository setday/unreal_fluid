/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : ultimate_py_project
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : Shader.cxx
 * PURPOSE   : ${PURPOSE}
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#include "Shader.h"

using namespace unreal_fluid::render;

Shader::Shader(Type _type) {
  unsigned int glType = 0;

  switch (_type) {
    case Type::VERTEX:
      glType = GL_VERTEX_SHADER;
      break;
    case Type::FRAGMENT:
      glType = GL_FRAGMENT_SHADER;
      break;
    case Type::GEOMETRY:
      glType = GL_GEOMETRY_SHADER;
      break;
    case Type::COMPUTE:
      glType = GL_COMPUTE_SHADER;
      break;
    default:
      break;
  }

  _shaderID = glCreateShader(glType);
}

Shader::~Shader() {
  glDeleteShader(_shaderID);
}

bool Shader::LoadProgram(std::string_view source) {
  const char *sourcePtr = source.data();

  glShaderSource(_shaderID, 1, &sourcePtr, nullptr);
  glCompileShader(_shaderID);

  GLint success;
  glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &success);

  return success;
}

GLuint Shader::GetShaderID() const {
  return _shaderID;
}

void Shader::GetLog(std::string &log) const {
  GLint logLength;
  glGetShaderiv(_shaderID, GL_INFO_LOG_LENGTH, &logLength);

  log.resize(logLength);
  glGetShaderInfoLog(_shaderID, logLength, nullptr, log.data());
}

// end of Shader.cxx