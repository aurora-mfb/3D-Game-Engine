#include "Core/InputManager.h"

#include <glm/gtc/matrix_transform.hpp>

InputManager::InputManager(GLFWwindow* window, std::shared_ptr<Camera> camera)
  : window(window), camera(camera) {
}

void InputManager::process(float deltaTime) {
  glm::vec3 pos = camera->getPosition();
  float speed = 5.0f * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    pos += speed * camera->getFront();
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    pos -= speed * camera->getFront();
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    pos -= speed * camera->getRight();
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    pos += speed * camera->getRight();

  camera->setPosition(pos);
}