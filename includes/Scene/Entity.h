#pragma once

#include "Graphics/Vertex.h"
#include "Core/State.h"

class Entity
{
public:
    Entity();
    Entity(glm::vec3& pos, glm::vec3& rot, glm::vec3& scale);
    virtual ~Entity();

    const glm::vec3& getPosition() const;
    void setPosition(const glm::vec3& pos);

    const glm::vec3& getRotation() const;
    void setRotation(const glm::vec3& rot);

    const glm::vec3& getScale() const;
    void setScale(const glm::vec3& scale);

    void move(const glm::vec3& vec);
    virtual void update(float deltaTime);
    virtual void draw();

private:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
};
