

#include "Scene/Camera.h"

#include "glew/GL/glew.h"


Camera::Camera(const glm::mat4& proj, const glm::ivec4& vp, const glm::vec3& color)
	: Entity(), m_projection(proj), m_viewport(vp), m_clearColor(color) {}

Camera::~Camera(){}

void Camera::prepare()
{
	// Set the global projection matrix state
	State::projectionMatrix = m_projection;

	// Define the target point and the up vector for the camera
	glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

	// Compute the view matrix based on the camera position and target
	State::viewMatrix = glm::lookAt(getPosition(), cameraTarget, cameraUp);

	// Configure the OpenGL viewport and scissor area
	glViewport(m_viewport.x, m_viewport.y, m_viewport.z, m_viewport.w);
	glScissor(m_viewport.x, m_viewport.y, m_viewport.z, m_viewport.w);

	// Set the color used to clear the color buffer
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.0f);

	// Enable depth testing so objects are rendered correctly in 3D
	glEnable(GL_DEPTH_TEST);

	// Clear both color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#pragma region Getters & Setters
glm::vec3 Camera::getFront() const 
{
	glm::vec3 rot = getRotation(); // Inherited from Entity
	float yaw = glm::radians(rot.y);   // Rotation around Y axis
	float pitch = glm::radians(rot.x); // Rotation around X axis

	glm::vec3 front;
	front.x = cos(pitch) * sin(yaw);
	front.y = sin(pitch);
	front.z = -cos(pitch) * cos(yaw);
	return glm::normalize(front);
}

glm::vec3 Camera::getRight() const 
{
	return glm::normalize(glm::cross(getFront(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

const glm::mat4& Camera::getProjection() const
{
	return m_projection;
}

void Camera::setProjection(const glm::mat4& proj)
{
	m_projection = proj;
}

const glm::ivec4& Camera::getViewport() const
{
	return m_viewport;
}

void Camera::setViewport(const glm::ivec4& vp)
{
	m_viewport = vp;
}

const glm::vec3& Camera::getClearColor() const
{
	return m_clearColor;
}

void Camera::setClearColor(const glm::vec3& color)
{
	m_clearColor = color;
}
#pragma endregion