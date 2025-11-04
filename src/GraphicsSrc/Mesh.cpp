
#define TINYOBJLOADER_IMPLEMENTATION

#include "Graphics/Mesh.h"
#include "Core/State.h"
#include <stdexcept>
#include <iostream>

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

  std::string modelPath = filename;
  std::string basePath = modelPath.substr(0, modelPath.find_last_of("/\\") + 1);

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
    modelPath.c_str(), basePath.c_str(), true)) {
    throw std::runtime_error(warn + err);
  }

  auto mesh = std::make_shared<Mesh>();
  std::shared_ptr<Shader> usedShader = shader ? shader : State::defaultShader;

  for (const auto& shape : shapes) {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    std::unordered_map<int, uint16_t> uniqueVertices; // evita duplicar vértices

    for (const auto& index : shape.mesh.indices) {
      float px = attrib.vertices[3 * index.vertex_index + 0];
      float py = attrib.vertices[3 * index.vertex_index + 1];
      float pz = attrib.vertices[3 * index.vertex_index + 2];

      float tu = 0.0f;
      float tv = 0.0f;
      if (index.texcoord_index >= 0) {
        tu = attrib.texcoords[2 * index.texcoord_index + 0];
        tv = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]; // invertir Y
      }

      // Creamos el vértice con posición, color blanco y coords textura
      Vertex vertex(px, py, pz, 1.0f, 1.0f, 1.0f, tu, tv);

      // Evitamos duplicados con la clave
      int packedIndex = index.vertex_index * 10000 + index.texcoord_index;
      if (uniqueVertices.count(packedIndex) == 0) {
        uniqueVertices[packedIndex] = static_cast<uint16_t>(vertices.size());
        vertices.push_back(vertex);
      }
      indices.push_back(uniqueVertices[packedIndex]);
    }

    // Crear el buffer con los vértices e índices
    auto buffer = std::make_shared<Buffer>(vertices, indices);

    // Material: si hay materiales en el OBJ, se carga la textura si existe
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

    Material material(texture, usedShader);
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

std::shared_ptr<Buffer>& Mesh::getBuffer(size_t index) {
    if (index >= buffers.size()) {
        throw std::out_of_range("Index out of range");
    }
    return buffers[index].first;
}

void Mesh::draw()
{
    if (buffers.empty()) {
      std::cerr << "[ERROR] La lista de buffers está vacía en Mesh::draw()." << std::endl;
      return;
    }

    for (const auto& BufferMaterialPair : buffers) {
        const std::shared_ptr<Buffer>& buffer = BufferMaterialPair.first;
        const Material& material = BufferMaterialPair.second;

        if (!buffer) {
          std::cerr << "[ERROR] Buffer nulo detectado en Mesh::draw()." << std::endl;
          continue;
        }
        if (!material.getShader()) {
          std::cerr << "[ERROR] Material sin shader válido detectado." << std::endl;
          continue;
        }

        // Configurar OpenGL con el material
        material.prepare();

        // Dibujar el buffer con el material activado
        buffer->draw(*material.getShader());

    }
}