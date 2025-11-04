#pragma once
#include "glfw/glfw3.h"
#include "memory"
#include "Scene/Camera.h"

class InputManager {
public:
  InputManager(GLFWwindow* window, std::shared_ptr<Camera> camera);
  void process(float deltaTime);
private:
  GLFWwindow* window;
  std::shared_ptr<Camera> camera;
};