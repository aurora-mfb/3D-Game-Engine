#pragma once


#include "Scene/Entity.h"
#include "Shader.h"
#include "glm/vec3.hpp"
#include "memory"

class Light : public Entity{

public:
  enum class Type {
    DIRECTIONAL = 0,
    POINT = 1
  };

private:
  Type type;
  glm::vec3 color;
  float linearAttenuation;

public:
  // Constructor
  Light(Type t = Type::POINT,
    const glm::vec3& col = glm::vec3(1.0f),
    float att = 0.0f);

  // Getters y setters
  Type getType() const;
  void setType(Type t);

  const glm::vec3& getColor() const;
  void setColor(const glm::vec3& col);

  float getLinearAttenuation() const;
  void setLinearAttenuation(float att);

  // ------------------------------------------------------------------------
  // Enviar datos al shader
  // index: índice de la luz (light[0], light[1], ...)
  // shader: shader activo al que escribimos uniformes
  // ------------------------------------------------------------------------
  void prepare(int index, std::shared_ptr<Shader>& shader) const;
};