#pragma once

#include <memory>
#include "../glm/glm.hpp"
#include "../lib/stb_image.h"

class Texture
{
public:

	Texture();
	~Texture();
	static std::shared_ptr<Texture> load(const char* filename);
	uint32_t getId() const;
	const glm::ivec2& getSize() const;
	void bind() const;

private:
	uint32_t Id;
	glm::ivec2 Size;

};