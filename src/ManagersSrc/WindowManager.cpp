#include "Managers/WindowManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
  // TODO: This won't work actually beacuse de camera function don't change its viewport.
}

bool WindowManager::init(int width, int height, const std::string& title) 
{
  // Initialize the GLFW library (used for creating windows and handling input)
  if (!glfwInit()) 
  {
    std::cerr << "[ERROR] Could not initialize GLFW" << std::endl;
    return false;
  }
  // Set window hints (e.g., enable multisampling for smoother edges)
  glfwWindowHint(GLFW_SAMPLES, 8);

  // Create the GLFW window with the specified width, height, and title
  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window) 
  {
    std::cerr << "[ERROR] Could not create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  // Make the OpenGL context of the created window the current one
  glfwMakeContextCurrent(window);

  // Enable sticky keys so GLFW remembers key states until polled
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  return true;
}

// Poll for window events (keyboard, mouse, resize, etc.)
void WindowManager::pollEvents() 
{ 
  glfwPollEvents(); 
}

// Swap the front and back buffers (update the displayed frame)
void WindowManager::swapBuffers() 
{ 
  glfwSwapBuffers(window); 
}

// Check if the user has requested to close the window
bool WindowManager::shouldClose() const 
{ 
  return glfwWindowShouldClose(window); 
}

// Properly destroy the window and terminate GLFW
void WindowManager::shutdown() 
{
  if (window) 
  {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
}