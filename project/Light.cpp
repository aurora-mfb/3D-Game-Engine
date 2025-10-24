#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "../lib/includes/Entity.h"
#include "../lib/includes/Shader.h"

class Light : public Entity {
public:
  // Enumerado para el tipo de luz
  enum class Type {
    DIRECTIONAL,
    POINT
  };

private:
  Type type;
  glm::vec3 color;
  float linearAttenuation;

public:
  Light()
    : type(Type::POINT), color(1.0f), linearAttenuation(0.0f) {
  }

  // Getters y Setters
  Type getType() const { return type; }
  void setType(Type t) { type = t; }

  const glm::vec3& getColor() const { return color; }
  void setColor(const glm::vec3& c) { color = c; }

  float getLinearAttenuation() const { return linearAttenuation; }
  void setLinearAttenuation(float att) { linearAttenuation = att; }

  // Método para pasar la información de la luz al shader
  void prepare(int index, std::shared_ptr<Shader>& shader) const {
    std::string prefix = "lights[" + std::to_string(index) + "].";

    // Tipo: 0 = DIRECTIONAL, 1 = POINT
    shader->setInt(prefix + "type", static_cast<int>(type));

    // Color de la luz
    shader->setVec3(prefix + "color", color);

    // Posición de la luz en espacio del observador
    glm::vec4 worldPos = getGlobalModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec3 viewPos = glm::vec3(shader->getViewMatrix() * worldPos);
    shader->setVec3(prefix + "position", viewPos);

    // Vector dirección para luces direccionales
    if (type == Type::DIRECTIONAL) {
      glm::vec4 worldDir = getGlobalModelMatrix() * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);  // Z- como dirección
      glm::vec3 viewDir = glm::vec3(shader->getViewMatrix() * worldDir);
      shader->setVec3(prefix + "direction", glm::normalize(viewDir));
    }

    // Atenuación
    shader->setFloat(prefix + "attenuation.constant", 1.0f);
    shader->setFloat(prefix + "attenuation.linear", linearAttenuation);
    shader->setFloat(prefix + "attenuation.quadratic", 0.0f);
  }
};