#pragma once

#include "Scene/World.h"
#include "Graphics/Shader.h"
#include "Scene/Camera.h"
#include "Scene/Model.h"
#include <memory>

class WorldBuilder {
public:
  static std::shared_ptr<World> createDefaultWorld(std::shared_ptr<Shader>& defaultShader, std::shared_ptr<Camera>& camera);
};