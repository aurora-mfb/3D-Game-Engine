#pragma once

#include "../lib/glew/GL/glew.h"
#include <vector>
#include "../lib/includes/Vertex.h"
#include "../lib/includes/Shader.h"

class Buffer
{
public:

	Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

	~Buffer();

	void draw(const Shader& shader) const;

private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLsizei m_IndexCount;
};