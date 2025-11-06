#pragma once

#include "Entity.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
 * @brief The Camera class represents a 3D camera in the scene.
 *				It manages projection, viewport configuration, and basic orientation vectors.
 *				It inherits position and rotation properties from the Entity class.
 */
class Camera: public Entity
{
public:

	/**
	 * @brief Constructor: Initializes the camera with a projection matrix, viewport, and clear color
	 * @param proj: Projection matrix
	 * @param vp:  Viewport
	 * @param color: Clear color.
	 */
	Camera(const glm::mat4& proj, const glm::ivec4& vp, const glm::vec3& color);

	/**
	 * @brief Destructor.
	 */
	~Camera();

	/* Prepares the OpenGL state before rendering */
	void prepare();

private:

#pragma region Private Variables
	// The projection matrix (perspective or orthographic projection).
	glm::mat4 m_projection;

	// The viewport rectangle: x, y, width, height.
	glm::ivec4 m_viewport;

	// The color used to clear the screen before rendering.
	glm::vec3 m_clearColor;
#pragma endregionç

public:

#pragma region Getters & Setters
	/* Calculates and returns the camera's "front" vector (the direction it is facing).
	 * Assumes that rotation is stored as Euler angles: pitch (x), yaw (y), roll (z) in degrees. */
	glm::vec3 getFront() const;

	/* Calculates and returns the camera's "right" vector
	 * (perpendicular to both the front vector and the global up vector). */
	glm::vec3 getRight() const;

	/* Returns the current projection matrix */
	const glm::mat4& getProjection() const;

	/* Sets a new projection matrix */
	void setProjection(const glm::mat4& proj);

	/* Returns the current viewport (x, y, width, height) */
	const glm::ivec4& getViewport() const;

	/* Sets a new viewport */
	void setViewport(const glm::ivec4& vp);

	/* Returns the clear color used when clearing the screen */
	const glm::vec3& getClearColor() const;

	/* Sets a new clear color */
	void setClearColor(const glm::vec3& color);
#pragma endregion

};