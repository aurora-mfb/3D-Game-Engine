#pragma once

#include "glew/GL/glew.h"
#include "GLFW/glfw3.h"
#include "string"
#include "iostream"

class WindowManager 
{

public:

  WindowManager() : window(nullptr) {}
  bool init(int width, int height, const std::string& title);
  void pollEvents();
  void swapBuffers();
  void shutdown();
  bool shouldClose() const;
  GLFWwindow* getWindow() const { return window; }

private:

  GLFWwindow* window;

};