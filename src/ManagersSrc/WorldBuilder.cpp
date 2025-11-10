#include "Managers/WorldBuilder.h"

std::shared_ptr<World> WorldBuilder::createDefaultWorld(std::shared_ptr<Shader>& defaultShader, std::shared_ptr<Camera>& camera) 
{
  // --- Shader ---
  // Create a default shader by loading vertex and fragment shader files.
  // This shader will be used to render all models in the world by default.
  defaultShader = std::make_shared<Shader>("../data/vertex.glsl", "../data/fragment.glsl");
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
  world->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));

  // --- Model ---
  // Load a 3D model from an OBJ file and associate it with the default shader.
  auto cowboyMesh = Mesh::load("../data/Models/gunslinger.obj", defaultShader);

  // Wrap the mesh in a Model entity and set its position in the world.
  auto cowboyModel = std::make_shared<Model>(cowboyMesh);
  cowboyModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

  // Add the model to the world so it will be drawn each frame.
  world->addEntity(cowboyModel);

  // Luz direccional blanca
  std::shared_ptr<Light> directionalLight = std::make_shared<Light>(Light::Type::DIRECTIONAL);
  directionalLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
  // Simulamos dirección (1,1,1) rotándola hacia abajo y un poco al lado
  directionalLight->setRotation(glm::vec3(-45.0f, 45.0f, 0.0f));
  world->addEntity(directionalLight);

  // Luz puntual roja (orbitará luego en Application::run)
  std::shared_ptr<Light> pointLight = std::make_shared<Light>(Light::Type::POINT);
  pointLight->setColor(glm::vec3(1.0f, 0.0f, 0.0f)); // rojo
  pointLight->setLinearAttenuation(0.2f);
  pointLight->setPosition(glm::vec3(2.0f, 2.0f, 0.0f)); // empieza a 5 unidades del modelo
  world->addEntity(pointLight);

  // Return the fully constructed world.
  return world;
}