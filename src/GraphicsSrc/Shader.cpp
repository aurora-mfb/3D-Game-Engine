
#include "Graphics/Shader.h"

#include "glew/GL/glew.h"

// Helper function to read an entire file into a string
std::string readString(const std::string& filename) 
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file) return "";

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string buffer(size, '\0');
	if (!file.read(&buffer[0], size)) return "";

	return buffer;
}

Shader::Shader(const char* cVertexShaderFile, const char* cFragmentShaderFile)
{
	// Read shader source code from files
	std::string sVertexShader = readString(cVertexShaderFile);
	std::string sFragmentShader = readString(cFragmentShaderFile);

	const char* vertexShaderSource = sVertexShader.c_str();
	const char* fragmentShaderSource = sFragmentShader.c_str();

	int retCode;

	// -------------------
	// Vertex Shader
	// -------------------
	m_uVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_uVertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(m_uVertexShader);
	glGetShaderiv(m_uVertexShader, GL_COMPILE_STATUS, &retCode);
	if(retCode == GL_FALSE) 
	{
		char errorLog[1024];
		glGetShaderInfoLog(m_uVertexShader, sizeof(errorLog), nullptr, errorLog);
		m_Error = errorLog;
		glDeleteShader(m_uVertexShader);
		return;
	}

	// -------------------
	// Fragment Shader
	// -------------------
	m_uFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_uFragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(m_uFragmentShader);

	// Check for compilation errors
	glGetShaderiv(m_uFragmentShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) 
	{
		char errorLog[1024];
		glGetShaderInfoLog(m_uFragmentShader, sizeof(errorLog), nullptr, errorLog);
		m_Error = errorLog;
		glDeleteShader(m_uFragmentShader);
		return;
	}

	// -------------------
	// Create Shader Program
	// -------------------
	m_Id = glCreateProgram();
	glAttachShader(m_Id, m_uVertexShader);
	glAttachShader(m_Id, m_uFragmentShader);
	glLinkProgram(m_Id);

	// Once linked, shaders can be deleted
	glDeleteShader(m_uVertexShader);
	glDeleteShader(m_uFragmentShader);

	// Check for linking errors
	glGetProgramiv(m_Id, GL_LINK_STATUS, &retCode);
	if(retCode == GL_FALSE) 
	{
		char errorLog[1024];
		glGetProgramInfoLog(m_Id, sizeof(errorLog), nullptr, errorLog);
		m_Error = errorLog;
		glDeleteProgram(m_Id);
		m_Id = 0;
		return;
	}

	// -------------------
	// Setup attribute locations
	// -------------------
	attribLocations["vPos"] = glGetAttribLocation(m_Id, "vpos");
	attribLocations["vColor"] = glGetAttribLocation(m_Id, "vcolor");
	attribLocations["vTex"] = glGetAttribLocation(m_Id, "vtex");
}

uint32_t Shader::getId() const
{
	return m_Id;
}

const char* Shader::getError() const 
{
	return m_Error;
}

void Shader::use() const
{
	glUseProgram(m_Id);
}

void Shader::setupAttribs() const 
{
	for (const auto& attrib : attribLocations) 
	{
		if (attrib.second != -1) // Only setup attributes that exist
		{ 
			glEnableVertexAttribArray(attrib.second);

			if (attrib.first == "vPos") // Vertex position (3 floats)
			{
				glVertexAttribPointer(attrib.second, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			}
			else if (attrib.first == "vColor") // Vertex color (3 floats)
			{
				glVertexAttribPointer(attrib.second, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
			}
			else if (attrib.first == "vTex") // Texture coordinates (2 floats)
			{ 
				glVertexAttribPointer(attrib.second, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoord));
			}
		}
	}
}

int Shader::getLocation(const char* name) const
{
	return glGetUniformLocation(m_Id, name); // Gets the location of a uniform variable in the shader
}

void Shader::setInt(int loc, int val)
{
	if (loc != -1)
	{
		glUniform1i(loc, val); // Sets the value of an integer uniform variable
	}
}

void Shader::setFloat(int loc, float val)
{
	if (loc != -1)
	{
		glUniform1f(loc, val); // Sets the value of a float uniform variable
	}
}

void Shader::setVec3(int loc, const glm::vec3& vec)
{
	if (loc != -1)
	{
		glUniform3f(loc, vec.x, vec.y, vec.z); // Sets the value of a vec3 uniform variable
	}
}

void Shader::setVec4(int loc, const glm::vec4& vec)
{
	if (loc != -1)
	{
		glUniform4f(loc, vec.x, vec.y, vec.z, vec.a); // Sets the value of a vec4 uniform variable
	}
}

void Shader::setMatrix(int loc, const glm::mat4& matrix)
{
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix)); // Sets the value of a mat4 uniform variable
	}
}
