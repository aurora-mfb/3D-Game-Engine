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
	void prepare() const;

private:

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;
};