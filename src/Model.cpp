

#include "../lib/includes/Model.h"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/includes/Mesh.h"

Model::Model(const std::shared_ptr<Mesh>& mesh):
	Entity(), m_mesh(mesh){}


void Model::draw() 
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, getPosition());

	model = glm::rotate(model, glm::radians(getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, getScale());

	State::modelMatrix = model;
	m_mesh->draw();
}