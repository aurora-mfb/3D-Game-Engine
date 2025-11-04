#pragma once
#include <memory>
#include "glm/detail/type_mat.hpp"
#include "Graphics/Shader.h"

class State
{
public:
	static std::shared_ptr<Shader> defaultShader;
	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 modelMatrix;

	static void init();
};