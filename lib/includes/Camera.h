#pragma once

#include "Entity.h"
#include "../glm/glm.hpp"
class Camera: public Entity
{
public:

	Camera();
	Camera(const glm::mat4& proj, const glm::ivec4& vp, const glm::vec3& color);
	~Camera();

	const glm::mat4& getProjection() const;
	void setProjection(const glm::mat4& proj);

	const glm::ivec4& getViewport() const;
	void setViewport(const glm::ivec4& vp);

	const glm::vec3& getClearColor() const;
	void setClearColor(const glm::vec3& color);

	void prepare();

	glm::vec3 getFront() const {
		// Asumiendo que rotación está en Euler: pitch, yaw, roll (en grados)
		glm::vec3 rot = getRotation(); // heredado de Entity
		float yaw = glm::radians(rot.y);   // rotación en Y
		float pitch = glm::radians(rot.x); // rotación en X

		glm::vec3 front;
		front.x = cos(pitch) * sin(yaw);
		front.y = sin(pitch);
		front.z = -cos(pitch) * cos(yaw);
		return glm::normalize(front);
	}

	glm::vec3 getRight() const {
		return glm::normalize(glm::cross(getFront(), glm::vec3(0.0f, 1.0f, 0.0f)));
	}

private:
	glm::mat4 m_projection;
	glm::ivec4 m_viewport;
	glm::vec3 m_clearColor;
};