#pragma once

#include "Entity.h"
#include "Camera.h"

/**
 * @brief The World class represents a container for all entities and cameras in a scene.
 *        It manages updating and rendering all the objects that exist within the world.
 *        It also inherits from Entity, allowing hierarchical transformations (e.g., a world offset).
 */
class World : public Entity
{
public:
  /**
   * @brief Constructor: initializes an empty world.
   */
  World();

  /**
   * @brief Destructor: cleans up resources if necessary.
   */
  ~World();

  /**
   * @brief Adds a new entity to the world. Entities are stored using smart pointers (shared_ptr) for automatic memory management.
   */
  void addEntity(const std::shared_ptr<Entity>& entity);

  /**
   * @brief Removes an existing entity from the world.
   */
  void removeEntity(const std::shared_ptr<Entity>& entity);

  /**
   * @brief Returns a constant reference to an entity at the specified index.
   */
  const std::shared_ptr<Entity>& getEntity(size_t index) const;

  /**
   * @brief Returns a modifiable reference to an entity at the specified index.
   */
  std::shared_ptr<Entity>& getEntity(size_t index);

  /**
   * @brief Updates all entities in the world based on the time elapsed since the last frame. Typically used for animations, movement, and physics.
   */
  void update(float deltaTime);

  /**
   * @brief Draws all entities in the world using the active camera(s). This method handles the rendering of the entire scene.
   */
  void draw();

private:
  /* A list of all entities currently present in the world. */
  std::vector<std::shared_ptr<Entity>> m_entities;

  /* A list of cameras associated with the world. Usually, one camera is active at a time, but multiple can exist (e.g., for split-screen). */
  std::vector<std::shared_ptr<Camera>> m_cameras;
};