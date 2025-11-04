#pragma once

#include <cstdint>
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include <unordered_map>
#include <string>


class Shader
{
public:

	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);

	// Devuelve el identificador de OpenGL del programa 
	uint32_t getId() const;
	// Obtiene el mensaje de error generado al compilar o enlazar 
	const char* getError() const;
	// Activa el uso de este programa 
	void use() const;
	// Activa la escritura de las variables attribute, 
	// y especifica su formato 
	void setupAttribs() const;

	// Obtiene la localización de una variable uniform 
	int getLocation(const char* name) const;
	// Da valor a una variable uniform 
	void setInt(int loc, int val);
	void setFloat(int loc, float val);
	void setVec3(int loc, const glm::vec3& vec);
	void setVec4(int loc, const glm::vec4& vec);
	void setMatrix(int loc, const glm::mat4& matrix);

private:
	uint32_t m_Id = 0;
	uint32_t m_uVertexShader = 0;
	uint32_t m_uFragmentShader = 0;
	const char* m_Error = nullptr;
	std::unordered_map<std::string, int> attribLocations;

};