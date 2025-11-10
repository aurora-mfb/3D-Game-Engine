#pragma once

#include <memory>
#include "Texture.h"
#include "Shader.h"

class Material
{
public:

	Material(const std::shared_ptr<Texture>& tex = nullptr, const std::shared_ptr<Shader>& shader = nullptr);
	~Material();

	const std::shared_ptr<Shader>& getShader() const;
	std::shared_ptr<Shader>& getShader();
	void setShader(const std::shared_ptr<Shader>& _shader);
	const std::shared_ptr<Texture>& getTexture() const;
	void setTexture(const std::shared_ptr<Texture>& _tex);
	const glm::vec4& getColor() const { return color; }
	void setColor(const glm::vec4& col) { color = col; }

	uint8_t getShininess() const { return shininess; }
	void setShininess(uint8_t s) { shininess = s; }
	void prepare() const;

private:

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;
	glm::vec4 color = glm::vec4(1.0f); // Color difuso
	uint8_t shininess = 30;            // brillo (0–255)
};