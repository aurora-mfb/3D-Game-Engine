#pragma once

#include <memory>
#include "Entity.h"
#include "Graphics/Mesh.h"

class Model: public Entity
{
public:

	Model(const std::shared_ptr<Mesh>& mes);
	virtual void draw() override;

private:

	std::shared_ptr<Mesh> m_mesh;
};