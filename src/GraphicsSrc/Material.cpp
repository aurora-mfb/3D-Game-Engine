
#include "Graphics/Material.h"

#include "Graphics/Light.h"
#include "Core/State.h"

Material::Material(const std::shared_ptr<Texture>& tex, const std::shared_ptr<Shader>& shader)
: texture(tex), shader(shader) {}

Material::~Material(){}

const std::shared_ptr<Shader>& Material::getShader() const
{
	return shader ? shader : State::defaultShader;
}

std::shared_ptr<Shader>& Material::getShader()
{
	return shader ? shader : State::defaultShader;
}

void Material::setShader(const std::shared_ptr<Shader>& _shader)
{
	shader = _shader;
}

const std::shared_ptr<Texture>& Material::getTexture() const
{
	return texture;
}

void Material::setTexture(const std::shared_ptr<Texture>& _tex)
{
	texture = _tex;
}

void Material::prepare() const 
{
  std::shared_ptr<Shader> activeShader = getShader();
  activeShader->use();

  // --- 1. Matrices ---
  glm::mat4 modelView = State::viewMatrix * State::modelMatrix;
  glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelView)));
  glm::mat4 mvp = State::projectionMatrix * modelView;

  activeShader->setMatrix(activeShader->getLocation("modelView"), modelView);
  activeShader->setMatrix(activeShader->getLocation("normalMatrix"), glm::mat4(normalMatrix));
  activeShader->setMatrix(activeShader->getLocation("mvp"), mvp);

  // --- 2. Textura ---
  int texSamplerLoc = activeShader->getLocation("texSampler");
  if (texture && texSamplerLoc != -1) {
    texture->bind();
    activeShader->setInt(texSamplerLoc, 0);
  }

  // --- 3. Material ---
  activeShader->setVec4(activeShader->getLocation("materialColor"), color);
  activeShader->setFloat(activeShader->getLocation("shininess"), shininess);

  // --- 4. Luz ambiente global ---
  activeShader->setVec3(activeShader->getLocation("ambient"), State::ambient);

  // --- 5. Luces ---
  int numLights = (int)State::lights.size();
  activeShader->setInt(activeShader->getLocation("numLights"), numLights);

  for (int i = 0; i < numLights; ++i)
    State::lights[i]->prepare(i, activeShader);
}