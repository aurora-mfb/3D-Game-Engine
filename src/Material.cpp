
#include "../includes/Material.h"
#include "../lib/includes/State.h"

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
	//Activamos shader:
	std::shared_ptr<Shader> activeShader = getShader();
	activeShader->use();

	//2.
	glm::mat4 mvpMatrix = State::projectionMatrix * State::viewMatrix * State::modelMatrix;

	int mvpLoc = -1;
	mvpLoc = activeShader->getLocation("mvp");
	activeShader->setMatrix(mvpLoc, mvpMatrix);

	//3.
	int texSamplerLoc = activeShader->getLocation("texSampler");
	if (texture && texSamplerLoc != -1) {
		texture->bind();
		activeShader->setInt(texSamplerLoc, 0);
	}

}