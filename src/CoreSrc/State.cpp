#include "Core/State.h"
#include "glm/gtc/matrix_transform.hpp"

std::shared_ptr<Shader> State::defaultShader = nullptr;
glm::mat4 State::projectionMatrix = glm::mat4(1.0f);
glm::mat4 State::viewMatrix = glm::mat4(1.0f);
glm::mat4 State::modelMatrix = glm::mat4(1.0f);
std::vector<std::shared_ptr<Light>> State::lights;
glm::vec3 State::ambient = glm::vec3(0.2f, 0.2f, 0.2f);

void State::init() {
    State::defaultShader = std::make_shared<Shader>("path_to_vertex_shader", "path_to_fragment_shader");

    State::projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    State::viewMatrix = glm::mat4(1.0f);
    State::modelMatrix = glm::mat4(1.0f);
}
