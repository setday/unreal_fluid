/***************************************************************
 * Copyright (C) 2023
 *    UnrealFluid Team (https://github.com/setday/unreal_fluid) and
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT                 : UnrealFluid
 * AUTHORS OF THIS PROJECT : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev.
 * FILE NAME               : Renderer.Main.cxx
 * FILE AUTHORS            : Serkov Alexander.
 *
 * No part of this file may be changed and used without
 * agreement of authors of this project.
 */

#include "Renderer.h"

using namespace unreal_fluid::render;

void GLAPIENTRY openglDebugInfo(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  Logger::logInfo("---------------------opengl-callback-start------------");
  Logger::logInfo("message:", message);

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      Logger::logInfo("type: ERROR");
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      Logger::logInfo("type: DEPRECATED_BEHAVIOR");
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      Logger::logInfo("type: UNDEFINED_BEHAVIOR");
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      Logger::logInfo("type: PORTABILITY");
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      Logger::logInfo("type: PERFORMANCE");
      break;
    case GL_DEBUG_TYPE_OTHER:
      Logger::logInfo("type: OTHER");
      break;
  }

  Logger::logInfo("id:", id);
  switch (severity){
    case GL_DEBUG_SEVERITY_LOW:
      Logger::logInfo("severity: LOW");
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      Logger::logInfo("severity: MEDIUM");
      break;
    case GL_DEBUG_SEVERITY_HIGH:
      Logger::logInfo("severity: HIGH");
      break;
  }

  Logger::logInfo("---------------------opengl-callback-end--------------");
}

Renderer::Renderer() {
  Logger::logInfo("Initializing renderer...");

  glewInit();

  _shaderManager = std::make_unique<ShaderManager>();

  camera = Camera({0.0f}, {0.0f, 0.0f, -1.0f});

  initGl();
  initBuffers();

  changeRenderMode(_renderMode);

  Logger::logInfo("Renderer initialized!");
} // end of Renderer::Renderer() function

void Renderer::initGl() const {
  /*
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(openglDebugInfo, nullptr);
  //*/

  // initialize viewport and ect
  glViewport(0, 0, 500, 500);

  glClearColor(0.00f, 0.00f, 0.00f, 0.0f);
  glClearDepth(1.0f);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(RESET_INDEX);

  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  // glShadeModel(GL_SMOOTH);

  // enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // enable alpha test
  // glEnable(GL_ALPHA_TEST);
  // glAlphaFunc(GL_GREATER, 0.1f);

  // enable texture 2d
  glEnable(GL_TEXTURE_2D);

  // set culling
  glCullFace(GL_BACK);

  // enable point smoothing
  // glEnable(GL_POINT_SMOOTH);
  // glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_MULTISAMPLE);
} // end of Renderer::initGL() function

void Renderer::initBuffers() {
  static float frameVertices[] = {
          -1.0f, -1.0f, 0.0f,0.0f, 0.0f,
          1.0f,  -1.0f, 0.0f,1.0f, 0.0f,
          -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
          1.0f,  1.0f, 0.0f, 1.0f, 1.0f
  };

  glGenVertexArrays(1, &_fvao);
  glGenBuffers(1, &_fvbo);

  glBindVertexArray(_fvao);
  glBindBuffer(GL_ARRAY_BUFFER, _fvbo);

  glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), frameVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo);
  glGenBuffers(1, &_ibo);

  glGenBuffers(1, &_rtubo);

  // depth
  _fbdt = std::make_unique<Texture>(500, 500, (std::size_t)5, sizeof(float));

  for (int i = 0; i < 5; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    _fbto[i] = std::make_unique<Texture>(500, 500);
  }

  glGenFramebuffers(1, &_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _fbdt->getID(), 0);

  for (int i = 0; i < 5; i++) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _fbto[i]->getID(), 0);
  }

  std::vector<unsigned int> attachments = {
          GL_COLOR_ATTACHMENT0,
          GL_COLOR_ATTACHMENT1,
          GL_COLOR_ATTACHMENT2,
          GL_COLOR_ATTACHMENT3,
          GL_COLOR_ATTACHMENT4,
  };
  glDrawBuffers((int)attachments.size(), attachments.data());

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // check framebuffer status
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    Logger::logError("Framebuffer is not complete:", status);
  }
}

Renderer::~Renderer() {
  glDisable(GL_DEPTH_TEST);

  glDeleteBuffers(1, &_vbo);
  glDeleteBuffers(1, &_ibo);
  glDeleteVertexArrays(1, &_vao);

  glDeleteBuffers(1, &_fvbo);
  glDeleteVertexArrays(1, &_fvao);

  glDeleteBuffers(1, &_rtubo);

  glDeleteFramebuffers(1, &_fbo);
} // end of Renderer::destroy() function

ShaderManager *Renderer::getShaderManager() const {
  return _shaderManager.get();
} // end of Renderer::getShaderManager() function

void Renderer::changeRenderMode(RenderMode mode) {
  _renderMode = mode;

  if (mode == RenderMode::WIREFRAME) {
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else if (mode == RenderMode::SOLID) {
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_FILL);
  } else if (mode == RenderMode::TEXTURED) {
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
} // end of Renderer::changeRenderMode() function

void Renderer::changeResolution(int width, int height) {
  camera.setResolution(width, height);

  // depth
  _fbdt->resize(width, height);

  for (const auto & i : _fbto)
    i->resize(width, height);

  glViewport(0, 0, width, height);
}

// end of Renderer.Main.cxx
