

#include <string>
#include <vector>

#include "glew/GL/glew.h"
#include "Graphics/Vertex.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffer.h"

Buffer::Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
	m_IndexCount = static_cast<GLsizei>(indices.size());

	// VAO:
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// VBO:
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// EBO:
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW);

	// Atributo 0: Posición (vpos) - 3 floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// Atributo 1: Color (vcolor) - 3 floats
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	// Atributo 2: Coordenadas de textura (vtex) - 2 floats
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoord));
	glEnableVertexAttribArray(2);

	// Desvincular el VAO (opcional)
	glBindVertexArray(0);

}

Buffer::~Buffer() 
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Buffer::draw(const Shader& shader) const 
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); // ?? asegura que esté vinculado
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}
