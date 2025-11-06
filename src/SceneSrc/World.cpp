#include "Scene/World.h"


World::World(): Entity(){}

World::~World(){}

void World::addEntity(const std::shared_ptr<Entity>& entity)
{
  // Store the entity in the main entity list
  m_entities.push_back(entity);

  // Check if the entity is actually a Camera (using dynamic cast)
  std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(entity);
  if (camera)
  {
    // If it’s a camera, add it to the camera list as well
    m_cameras.push_back(camera);
  }
}

void World::removeEntity(const std::shared_ptr<Entity>& entity)
{
  // Find and remove the entity from the main list
  auto it = std::find(m_entities.begin(), m_entities.end(), entity);
  if (it != m_entities.end())
  {
    m_entities.erase(it);
  }

  // Check if this entity is a Camera and remove it from the camera list
  std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(entity);
  if (camera)
  {
    auto cameraIt = std::find(m_cameras.begin(), m_cameras.end(), camera);
    if (cameraIt != m_cameras.end())
    {
      m_cameras.erase(cameraIt);
    }
  }
}

const std::shared_ptr<Entity>& World::getEntity(size_t index) const
{
  return m_entities.at(index);
}

std::shared_ptr<Entity>& World::getEntity(size_t index)
{
  return m_entities.at(index);
}

void World::update(float deltaTime)
{
  for (auto& entity : m_entities)
  {
      entity->update(deltaTime);
  }
}

void World::draw()
{
  // Prepare each camera (sets up projection, view matrices, clears buffers, etc.)
  for (auto& camera : m_cameras)
  {
    camera->prepare();
  }

  // Draw all entities (meshes, models, etc.)
  for (auto& entity : m_entities)
  {
    entity->draw();
  }
}