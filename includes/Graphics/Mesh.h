#pragma once

#include <vector>
#include <memory>
#include "Buffer.h"
#include "Material.h"

#include "../lib/tinyobjloader-master/tiny_obj_loader.h"

class Mesh
{
public:

	void addBuffer(const std::shared_ptr<Buffer>& buffer, const Material& material);

	static std::shared_ptr<Mesh> load(const char* filename, const std::shared_ptr<Shader>& shader = nullptr);
	size_t getNumBuffers() const;
	const std::shared_ptr<Buffer>& getBuffer(size_t index) const;
	std::shared_ptr<Buffer>& getBuffer(size_t index);
	void draw();

private:

	std::vector<std::pair<std::shared_ptr<Buffer>, Material>> buffers;
};