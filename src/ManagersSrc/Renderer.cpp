#include "Managers/Renderer.h"

bool Renderer::init() 
{
  // Initialize GLEW (OpenGL Extension Wrangler Library)
  // This loads all OpenGL function pointers based on the active context created by GLFW.
  GLenum err = glewInit();

  // Check if GLEW initialization failed
  if (err != GLEW_OK) {
    std::cerr << "Error al inicializar GLEW: " << glewGetErrorString(err) << std::endl;
    return false;
  }

  // Enable depth testing to ensure that closer objects obscure farther ones.
  glEnable(GL_DEPTH_TEST);

  // Enable scissor testing to restrict rendering to a specific rectangular region. 
  // If not define with glScissor(...) it will affect all the screen.
  glEnable(GL_SCISSOR_TEST);

  // If everything succeeded, return true.
  return true;
}

void Renderer::clear() 
{
  // Clear both the color buffer and the depth buffer before drawing a new frame.
  // This ensures that old frame data does not persist on screen.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}