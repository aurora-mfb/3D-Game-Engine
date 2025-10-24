
#include "../lib/includes/Shader.h"
#include "../lib/glew/GL/glew.h"
#include "../lib/glm/gtc/type_ptr.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "../lib/includes/Vertex.h"


std::string readString(const std::string& filename) {
	std::ifstream istream(filename.c_str(), std::ios_base::binary);
	std::stringstream sstream;
	sstream << istream.rdbuf();
	return sstream.str();
}


Shader::Shader(const char* cVertexShaderFile, const char* cFragmentShaderFile)
{
	// Leemos ficheros:
	std::string sVertexShader = readString(cVertexShaderFile);
	std::string sFragmentShader = readString(cFragmentShaderFile);

	const char* vertexShaderSource = sVertexShader.c_str();
	const char* fragmentShaderSource = sFragmentShader.c_str();

	int retCode;

	//Vertex Shader
	m_uVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_uVertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(m_uVertexShader);
	glGetShaderiv(m_uVertexShader, GL_COMPILE_STATUS, &retCode);
	if(retCode == GL_FALSE) {
		char errorLog[1024];
		glGetShaderInfoLog(m_uVertexShader, sizeof(errorLog), nullptr, errorLog);
		m_Error = errorLog;
		glDeleteShader(m_uVertexShader);
		return;
	}

	//Fragment Shader
	m_uFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_uFragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(m_uFragmentShader);
	glGetShaderiv(m_uFragmentShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		char errorLog[1024];
		glGetShaderInfoLog(m_uFragmentShader, sizeof(errorLog), nullptr, errorLog);
		m_Error = errorLog;
		glDeleteShader(m_uFragmentShader);
		return;
	}

	//Creación del programa:
	m_Id = glCreateProgram();
	glAttachShader(m_Id, m_uVertexShader);
	glAttachShader(m_Id, m_uFragmentShader);
	glLinkProgram(m_Id);
	glDeleteShader(m_uVertexShader);
	glDeleteShader(m_uFragmentShader);
	glGetProgramiv(m_Id, GL_LINK_STATUS, &retCode);
	if(retCode == GL_FALSE) {
		char errorLog[1024];
		glGetProgramInfoLog(m_Id, sizeof(errorLog), nullptr, errorLog);
		m_Error = errorLog;
		glDeleteProgram(m_Id);
		m_Id = 0;
		return;
	}


	//Seteamos atributos:
	attribLocations["vPos"] = glGetAttribLocation(m_Id, "vpos");
	attribLocations["vColor"] = glGetAttribLocation(m_Id, "vcolor");
	attribLocations["vTex"] = glGetAttribLocation(m_Id, "vtex");

}

// Devuelve el identificador de OpenGL del programa 
uint32_t Shader::getId() const
{
	return m_Id;
}

// Obtiene el mensaje de error generado al compilar o enlazar 
const char* Shader::getError() const 
{
	return m_Error;
}

// Activa el uso de este programa 
void Shader::use() const
{
	glUseProgram(m_Id);
}

// Activa la escritura de las variables attribute, 
// y especifica su formato 
void Shader::setupAttribs() const {
	for (const auto& attrib : attribLocations) {
		if (attrib.second != -1) { // Verificar que la localización no sea -1
			glEnableVertexAttribArray(attrib.second);
			if (attrib.first == "vPos") {
				glVertexAttribPointer(attrib.second, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			}
			else if (attrib.first == "vColor") {
				glVertexAttribPointer(attrib.second, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
			}
			else if (attrib.first == "vTex") { // NUEVO: Configurar las coordenadas de textura
				glVertexAttribPointer(attrib.second, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoord));
			}
		}
	}
}

// Obtiene la localización de una variable uniform 
int Shader::getLocation(const char* name) const
{
	return glGetUniformLocation(m_Id, name);
}

// Da valor a una variable uniform 
void Shader::setInt(int loc, int val)
{
	if (loc != -1)
	{
		glUniform1i(loc, val);
	}
}

void Shader::setFloat(int loc, float val)
{
	if (loc != -1)
	{
		glUniform1f(loc, val);
	}
}

void Shader::setVec3(int loc, const glm::vec3& vec)
{
	if (loc != -1)
	{
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}
}

void Shader::setVec4(int loc, const glm::vec4& vec)
{
	if (loc != -1)
	{
		glUniform4f(loc, vec.x, vec.y, vec.z, vec.a);
	}
}

void Shader::setMatrix(int loc, const glm::mat4& matrix)
{
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
