#include "Graphics/Light.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Light::Light(Type t,
  const glm::vec3& col,
  float att)
  : type(t), color(col), linearAttenuation(att) {}

// Getters y setters
Light::Type Light::getType() const { return type; }
void Light::setType(Type t) { type = t; }

const glm::vec3& Light::getColor() const { return color; }
void Light::setColor(const glm::vec3& col) { color = col; }

float Light::getLinearAttenuation() const { return linearAttenuation; }
void Light::setLinearAttenuation(float att) { linearAttenuation = att; }

void Light::prepare(int index, std::shared_ptr<Shader>& shader) const {
  std::string prefix = "lights[" + std::to_string(index) + "].";

  // --- Tipo ---
  int locType = shader->getLocation((prefix + "type").c_str());
  shader->setInt(locType, static_cast<int>(type));

  // --- Color ---
  int locColor = shader->getLocation((prefix + "color").c_str());
  shader->setVec3(locColor, color);

  // --- Posición / Dirección ---
  glm::vec3 worldPos = getWorldPosition();
  glm::vec3 forward = getForwardVector();

  if (type == Type::DIRECTIONAL)
  {
    // Para direccionales, solo enviamos dirección normalizada
    int locDir = shader->getLocation((prefix + "direction").c_str());
    shader->setVec3(locDir, glm::normalize(forward));
  }
  else if (type == Type::POINT)
  {
    int locPos = shader->getLocation((prefix + "position").c_str());
    shader->setVec3(locPos, worldPos);

    // Atenuaciones
    int locLin = shader->getLocation((prefix + "linearAttenuation").c_str());
    int locConst = shader->getLocation((prefix + "constantAttenuation").c_str());
    int locQuad = shader->getLocation((prefix + "quadraticAttenuation").c_str());

    shader->setFloat(locLin, linearAttenuation);
    shader->setFloat(locConst, 1.0f);
    shader->setFloat(locQuad, 0.0f);
  }
}