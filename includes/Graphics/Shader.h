#pragma once

// Utils
#include "unordered_map"
#include "fstream"
#include "sstream"

// Libs
#include "glm/gtc/type_ptr.hpp"

// Includes
#include "Graphics/Vertex.h"

class Shader
{
public:

	// Constructor that takes vertex and fragment shader source code
	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);

	// Returns the OpenGL program ID
	uint32_t getId() const;

	// Gets the error message generated during compilation or linking
	const char* getError() const;

	// Activates this shader program for use
	void use() const;

	// Sets up the attribute variables and specifies their format
	void setupAttribs() const;

	// Gets the location of a uniform variable
	int getLocation(const char* name) const;

	// Sets the value of a uniform variable
	void setInt(int loc, int val);
	void setFloat(int loc, float val);
	void setVec3(int loc, const glm::vec3& vec);
	void setVec4(int loc, const glm::vec4& vec);
	void setMatrix(int loc, const glm::mat4& matrix);

private:

	// OpenGL program ID
	uint32_t m_Id = 0; 

	// Vertex shader ID
	uint32_t m_uVertexShader = 0; 

	// Fragment shader ID
	uint32_t m_uFragmentShader = 0; 

	// Compilation or linking error message
	const char* m_Error = nullptr; 

	// Attribute locations cache
	std::unordered_map<std::string, int> attribLocations; 

};