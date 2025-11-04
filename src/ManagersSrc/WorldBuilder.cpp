#include "Managers/WorldBuilder.h"

std::shared_ptr<World> WorldBuilder::createDefaultWorld(std::shared_ptr<Shader>& defaultShader, std::shared_ptr<Camera>& camera) 
{
  // --- Shader ---
  // Create a default shader by loading vertex and fragment shader files.
  // This shader will be used to render all models in the world by default.
  defaultShader = std::make_shared<Shader>("data/vertex.glsl", "data/fragment.glsl");
  if (defaultShader->getId() == 0)
    throw std::runtime_error("Error al crear el shader.");

  // --- Global Matrices ---
  // Set up a perspective projection matrix for the world.
  // 60 degree field of view, 800x600 aspect ratio, near=0.1, far=100 units.
  glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.f / 600.f, 0.1f, 100.f);

  // Store projection and view matrices in a global state object.
  // These matrices will be used for camera transforms and rendering.
  State::projectionMatrix = projection;
  State::viewMatrix = glm::mat4(1.0f);

  // --- Camera ---
  // Create a camera that will view the 3D world.
  glm::ivec4 viewport(0, 0, 800, 600);
  glm::vec3 clearColor(0.1f, 0.1f, 0.1f);
  camera = std::make_shared<Camera>(projection, viewport, clearColor);

  // Position the camera slightly back and up so it can see the scene.
  camera->setPosition(glm::vec3(0.0f, 1.0f, -40.0f));

  // --- World ---
  // Create a new World object that will hold all entities (models, camera, etc.)
  auto world = std::make_shared<World>();

  // Add the camera to the world so it can be used for rendering.
  world->addEntity(camera);

  // --- Model ---
  // Load a 3D model from an OBJ file and associate it with the default shader.
  auto cowboyMesh = Mesh::load("data/Models/gunslinger.obj", defaultShader);

  // Wrap the mesh in a Model entity and set its position in the world.
  auto cowboyModel = std::make_shared<Model>(cowboyMesh);
  cowboyModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

  // Add the model to the world so it will be drawn each frame.
  world->addEntity(cowboyModel);

  // Return the fully constructed world.
  return world;
}