#pragma once

// Utils:
#include "memory"
#include "glm/gtc/matrix_transform.hpp"
#include "stdexcept"

// Graphics:
#include "Graphics/Shader.h"

// Scene:
#include "Scene/World.h"
#include "Scene/Camera.h"
#include "Scene/Model.h"

class WorldBuilder 
{

public:

  static std::shared_ptr<World> createDefaultWorld(std::shared_ptr<Shader>& defaultShader, std::shared_ptr<Camera>& camera);

};