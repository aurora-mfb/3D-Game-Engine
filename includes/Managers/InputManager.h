#pragma once

// Utils
#include "glfw/glfw3.h"
#include "memory"

// Scene
#include "Scene/Camera.h"

class InputManager 
{
public:

  static void handleInput(GLFWwindow* window, std::shared_ptr<Camera> camera, float deltaTime);

};