#include "Managers/WorldBuilder.h"

#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

std::shared_ptr<World> WorldBuilder::createDefaultWorld(std::shared_ptr<Shader>& defaultShader, std::shared_ptr<Camera>& camera) {
  // Shader
  defaultShader = std::make_shared<Shader>("data/vertex.glsl", "data/fragment.glsl");
  if (defaultShader->getId() == 0)
    throw std::runtime_error("Error al crear el shader.");

  // Matrices
  glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.f / 600.f, 0.1f, 100.f);
  State::projectionMatrix = projection;
  State::viewMatrix = glm::mat4(1.0f);

  // Cámara
  glm::ivec4 viewport(0, 0, 800, 600);
  glm::vec3 clearColor(0.1f, 0.1f, 0.1f);
  camera = std::make_shared<Camera>(projection, viewport, clearColor);
  camera->setPosition(glm::vec3(0.0f, 1.0f, -20.0f));

  // Mundo
  auto world = std::make_shared<World>();
  world->addEntity(camera);

  // Modelo
  auto cowboyMesh = Mesh::load("data/Models/gunslinger.obj", defaultShader);
  auto cowboyModel = std::make_shared<Model>(cowboyMesh);
  cowboyModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
  world->addEntity(cowboyModel);

  return world;
}