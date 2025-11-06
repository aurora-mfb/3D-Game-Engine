#include "Managers/InputManager.h"

void InputManager::handleInput(GLFWwindow* window, std::shared_ptr<Camera> camera, float deltaTime) 
{
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
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);


  camera->setPosition(pos);
}