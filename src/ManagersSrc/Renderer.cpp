#include "Managers/Renderer.h"

bool Renderer::init() {
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "Error al inicializar GLEW: " << glewGetErrorString(err) << std::endl;
    return false;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  return true;
}

void Renderer::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}