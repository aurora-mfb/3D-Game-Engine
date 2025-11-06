#pragma once

// Utils
#include "iostream"

// Headers
#include "Core/State.h"
#include "Buffer.h"
#include "Material.h"

// Libs
#include "tinyobjloader-master/tiny_obj_loader.h"

/**
 * @brief The Mesh class represents a 3D model composed of one or more vertex buffers
 *				(geometry data) and associated materials (shaders, textures, colors, etc.).
 *				It provides methods for loading, managing, and drawing mesh data.
 */
class Mesh
{
public:

	/**
	 * @brief Adds a new sub-mesh (a vertex buffer and its corresponding material) to this mesh.
	 *				 Each pair of Buffer + Material represents a renderable part of the model.
	 *				 For example, a complex object like a car can have multiple buffers,
	 *				 each with a different material (body, glass, wheels, etc.).
	 * @param buffer A shared pointer to the vertex buffer containing geometry data.
	 * @param material The material that defines how this geometry is rendered.
	 */
	void addBuffer(const std::shared_ptr<Buffer>& buffer, const Material& material);

	/**
	 * @brief Loads a mesh from an OBJ file using the TinyObjLoader library.
	 *				This method parses the geometry and material data from the specified file,
	 *				creates vertex buffers, and constructs a Mesh instance containing them.
	 *				An optional shader can be assigned to the materials of the loaded mesh.
	 * @param filename The path to the OBJ file to load.
	 * @param shader (Optional) A shared pointer to a shader to assign to all materials.
	 * @return A shared pointer to a newly created Mesh containing the loaded data.
	 */
	static std::shared_ptr<Mesh> load(const char* filename, const std::shared_ptr<Shader>& shader = nullptr);

	/**
	 * @brief Gets the total number of vertex buffers (sub-meshes) in this mesh.
	 * @return The number of buffers currently stored in the mesh.
	 */
	size_t getNumBuffers() const;

	/**
	 * @brief Retrieves a constant reference to a vertex buffer at the specified index.
	 *				This allows read-only access to the buffer without modifying it.
	 * @param index The zero-based index of the buffer to retrieve.
	 * @return A constant shared pointer reference to the requested buffer.
	 */
	const std::shared_ptr<Buffer>& getBuffer(size_t index) const;

	/**
	 * @brief Retrieves a modifiable reference to a vertex buffer at the specified index.
	 *				This allows you to modify the buffer or its data after it has been added.
	 * @param index The zero-based index of the buffer to retrieve.
	 * @return A shared pointer reference to the requested buffer.
	 */
	std::shared_ptr<Buffer>& getBuffer(size_t index);

	/**
	 * @brief Draws the mesh by rendering each buffer with its assigned material.
	 *				This function iterates through all the stored (Buffer, Material) pairs,
	 *				binds their corresponding OpenGL resources, and issues draw calls.
	 */
	void draw();

private:

	/**
	 * @brief A list of pairs, where each pair contains:
	 * - A vertex buffer (geometry data)
	 * - Its associated material (shader, color, texture, etc.)
	 * Together, they represent the visual components that make up the entire mesh.
	 */
	std::vector<std::pair<std::shared_ptr<Buffer>, Material>> buffers;
};