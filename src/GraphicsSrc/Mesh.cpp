#define TINYOBJLOADER_IMPLEMENTATION

#include "Graphics/Mesh.h"

void Mesh::addBuffer(const std::shared_ptr<Buffer>& buffer, const Material& material)
{
  buffers.push_back({ buffer, material });
}

std::shared_ptr<Mesh> Mesh::load(const char* filename, const std::shared_ptr<Shader>& shader)
{
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  // Extract model path and base directory (used to locate texture files)
  std::string modelPath = filename;
  std::string basePath = modelPath.substr(0, modelPath.find_last_of("/\\") + 1);

  // Load the OBJ file and its materials
  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
    modelPath.c_str(), basePath.c_str(), true)) {
    throw std::runtime_error(warn + err);
  }

  // Create a new mesh instance
  auto mesh = std::make_shared<Mesh>();
  std::shared_ptr<Shader> usedShader = shader ? shader : State::defaultShader;

  // Iterate through each shape (object or group) in the file
  for (const auto& shape : shapes) {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    // Map to avoid duplicating vertices that share position/texture coordinates
    std::unordered_map<int, uint16_t> uniqueVertices;

    // Process all vertex indices in this shape
    for (const auto& index : shape.mesh.indices) {
      // Read vertex position
      float px = attrib.vertices[3 * index.vertex_index + 0];
      float py = attrib.vertices[3 * index.vertex_index + 1];
      float pz = attrib.vertices[3 * index.vertex_index + 2];

      // Read texture coordinates (if they exist)
      float tu = 0.0f;
      float tv = 0.0f;
      if (index.texcoord_index >= 0) {
        tu = attrib.texcoords[2 * index.texcoord_index + 0];
        tv = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]; // Flip Y
      }

      // Create the vertex with position, white color, and texture coordinates
      Vertex vertex(px, py, pz, 1.0f, 1.0f, 1.0f, tu, tv);

      // Create a unique key combining vertex and texcoord indices
      int packedIndex = index.vertex_index * 10000 + index.texcoord_index;

      // Add the vertex only if it hasn’t been seen before
      if (uniqueVertices.count(packedIndex) == 0) {
        uniqueVertices[packedIndex] = static_cast<uint16_t>(vertices.size());
        vertices.push_back(vertex);
      }

      // Add the index (points to the unique vertex)
      indices.push_back(uniqueVertices[packedIndex]);
    }

    // Create the GPU buffer with the vertex and index data
    auto buffer = std::make_shared<Buffer>(vertices, indices);

    // Try to load a texture from the material (if any)
    std::shared_ptr<Texture> texture = nullptr;
    if (!materials.empty() && !shape.mesh.material_ids.empty()) {
      int matId = shape.mesh.material_ids[0];
      if (matId >= 0 && matId < materials.size()) {
        std::string texFilename = materials[matId].diffuse_texname;
        if (texFilename.empty()) texFilename = materials[matId].ambient_texname;
        if (!texFilename.empty()) {
          texture = Texture::load((basePath + texFilename).c_str());
        }
      }
    }

    // Create a material (with texture and shader)
    Material material(texture, usedShader);

    // Add this buffer-material pair to the mesh
    mesh->addBuffer(buffer, material);
  }

  return mesh;
}

size_t Mesh::getNumBuffers() const
{
  return buffers.size();
}

const std::shared_ptr<Buffer>& Mesh::getBuffer(size_t index) const 
{
  if (index >= buffers.size()) {
      throw std::out_of_range("Index out of range");
  }
  return buffers[index].first;
}

std::shared_ptr<Buffer>& Mesh::getBuffer(size_t index) 
{
  if (index >= buffers.size()) {
      throw std::out_of_range("Index out of range");
  }
  return buffers[index].first;
}

void Mesh::draw()
{
  // Safety check: no buffers to draw
  if (buffers.empty()) {
    std::cerr << "[ERROR] The buffer list is empty in Mesh::draw()." << std::endl;
    return;
  }

  // Iterate over all Buffer-Material pairs
  for (const auto& BufferMaterialPair : buffers) {
    const std::shared_ptr<Buffer>& buffer = BufferMaterialPair.first;
    const Material& material = BufferMaterialPair.second;

    // Validate the buffer and material before drawing
    if (!buffer) {
      std::cerr << "[ERROR] Null buffer detected in Mesh::draw()." << std::endl;
      continue;
    }
    if (!material.getShader()) {
      std::cerr << "[ERROR] Material without a valid shader detected." << std::endl;
      continue;
    }

    // Prepare the material (binds shader, textures, uniforms, etc.)
    material.prepare();

    // Draw the vertex buffer using the active shader
    buffer->draw(*material.getShader());
  }
}