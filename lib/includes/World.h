#pragma once

#include <memory>
#include <vector>
#include "Entity.h"
#include "Camera.h"

class World : public Entity
{
public:
    World();
    ~World();

    void addEntity(const std::shared_ptr<Entity>& entity);
    void removeEntity(const std::shared_ptr<Entity>& entity);

    const std::shared_ptr<Entity>& getEntity(size_t index) const;
    std::shared_ptr<Entity>& getEntity(size_t index);

    void update(float deltaTime);

    void draw();

private:
    std::vector<std::shared_ptr<Entity>> m_entities;
    std::vector<std::shared_ptr<Camera>> m_cameras;
};
