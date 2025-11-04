#include "Managers/WindowManager.h"

bool WindowManager::init(int width, int height, const std::string& title) {
  if (!glfwInit()) {
    std::cerr << "[ERROR] Could not initialize GLFW" << std::endl;
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 8);
  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window) {
    std::cerr << "[ERROR] Could not create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
  return true;
}

void WindowManager::pollEvents() { glfwPollEvents(); }
void WindowManager::swapBuffers() { glfwSwapBuffers(window); }
bool WindowManager::shouldClose() const { return glfwWindowShouldClose(window); }

void WindowManager::shutdown() {
  if (window) {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
}