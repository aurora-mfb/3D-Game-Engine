

#include "Scene/World.h"

World::World(): Entity(){}

World::~World(){}

void World::addEntity(const std::shared_ptr<Entity>& entity)
{
	m_entities.push_back(entity);

    std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(entity);
    if (camera)
    {
        m_cameras.push_back(camera);
    }
}

void World::removeEntity(const std::shared_ptr<Entity>& entity)
{
    auto it = std::find(m_entities.begin(), m_entities.end(), entity);
    if (it != m_entities.end())
    {
        m_entities.erase(it);
    }

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
    for (auto& camera : m_cameras)
    {
        camera->prepare();
    }

    for (auto& entity : m_entities)
    {
        entity->draw();
    }
}