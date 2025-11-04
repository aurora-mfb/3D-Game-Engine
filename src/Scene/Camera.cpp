

#include "Scene/Camera.h"
#include "glew/GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"


Camera::Camera(const glm::mat4& proj, const glm::ivec4& vp, const glm::vec3& color)
	: Entity(), m_projection(proj), m_viewport(vp), m_clearColor(color) {}

Camera::~Camera(){}

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

void Camera::prepare()
{
	State::projectionMatrix = m_projection;

	glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

	//uso del getRotation?
	State::viewMatrix = glm::lookAt(getPosition(), cameraTarget, cameraUp);

	glViewport(m_viewport.x, m_viewport.y, m_viewport.z, m_viewport.w);
	glScissor(m_viewport.x, m_viewport.y, m_viewport.z, m_viewport.w);
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}