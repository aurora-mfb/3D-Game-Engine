#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glew/GL/glew.h"
#include "glfw/glfw3.h"
#include "Graphics/Vertex.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffer.h"
#include <iostream>
#include <vector>
#include "Graphics/Mesh.h"
#include "Scene/Camera.h"
#include "Scene/Model.h"
#include "Scene/World.h"
#include "Graphics/Material.h"
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

  GLFWwindow* window;
  std::shared_ptr<World> world;
  std::shared_ptr<Camera> camera;
  std::shared_ptr<InputManager> input;
  std::shared_ptr<WindowManager> windowManager;
  std::unique_ptr<Renderer> renderer;
  int width, height;
  std::string title;
};