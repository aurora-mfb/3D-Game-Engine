
#include "Scene/Entity.h"

Entity::Entity()
	: m_position(0.0f, 0.0f, 0.0f),
	m_rotation(0.0f, 0.0f, 0.0f),
	m_scale(1.0f, 1.0f, 1.0f)
{}

Entity::Entity(glm::vec3& pos, glm::vec3& rot, glm::vec3& scale):
	m_position(pos), m_rotation(rot), m_scale(scale){}

Entity::~Entity(){}

const glm::vec3& Entity::getPosition() const
{
	return m_position;
}

void Entity::setPosition(const glm::vec3& pos)
{
	m_position = pos;
}

const glm::vec3& Entity::getRotation() const
{
	return m_rotation;
}

void Entity::setRotation(const glm::vec3& rot)
{
	m_rotation = rot;
}

const glm::vec3& Entity::getScale() const 
{
	return m_scale;
}

void Entity::setScale(const glm::vec3& scale)
{
	m_scale = scale;
}

void Entity::update(float deltaTime)
{

}

void Entity::draw()
{

}