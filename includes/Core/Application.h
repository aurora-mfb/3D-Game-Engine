#pragma once

// Utils:
#include "glew/GL/glew.h"

// Managers
#include "Managers/InputManager.h"
#include "Managers/WindowManager.h"
#include "Managers/Renderer.h"
#include "Managers/WorldBuilder.h"

class Application {
public:

  Application(int width, int height, const std::string& title);
  bool init();
  void run();
  void shutdown();

private:

  // Window parameters:
  GLFWwindow* window;
  int width, height;
  std::string title;

  // Pointers parameters:
  std::shared_ptr<World> world;
  std::shared_ptr<Camera> camera;
  std::shared_ptr<InputManager> input;
  std::shared_ptr<WindowManager> windowManager;
  std::unique_ptr<Renderer> renderer;
};