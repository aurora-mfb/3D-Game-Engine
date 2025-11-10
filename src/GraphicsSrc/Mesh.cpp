#define TINYOBJLOADER_IMPLEMENTATION

#include "Graphics/Mesh.h"
#include <fstream>
#include <iostream>
#include <filesystem> // <-- este es el importante

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
  std::ifstream fileCheck(filename);
  if (!fileCheck.is_open()) {
    std::cerr << "[ERROR] No se encontró el archivo OBJ: " << filename << std::endl;
    char buffer[512];
    _fullpath(buffer, filename, 512);
    std::cerr << "Ruta absoluta: " << buffer << std::endl;
    throw std::runtime_error("El archivo OBJ no existe o la ruta es incorrecta.");
  }
  fileCheck.close();

  std::cout << "[INFO] Cargando modelo: " << filename << std::endl;
  std::cout << "Base path: " << basePath << std::endl;

  // Intentamos cargar el modelo
  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
    modelPath.c_str(), basePath.c_str(), true)) {
    std::cerr << "[ERROR] Fallo al cargar el modelo OBJ: " << filename << std::endl;
    if (!warn.empty()) std::cerr << "Aviso: " << warn << std::endl;
    if (!err.empty())  std::cerr << "Error: " << err << std::endl;

    throw std::runtime_error("Error al cargar el modelo: " + warn + err);
  }

  std::cout << "[OK] Modelo cargado correctamente: " << filename << std::endl;
  std::cout << " - Nº de vértices: " << attrib.vertices.size() / 3 << std::endl;
  std::cout << " - Nº de shapes: " << shapes.size() << std::endl;
  std::cout << " - Nº de materiales: " << materials.size() << std::endl;


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
      float nx = 0.0f;
      float ny = 0.0f;
      float nz = 1.0f;

      if (index.normal_index >= 0) {
        nx = attrib.normals[3 * index.normal_index + 0];
        ny = attrib.normals[3 * index.normal_index + 1];
        nz = attrib.normals[3 * index.normal_index + 2];
      }

      // Crear vertex con normales
      Vertex vertex(px, py, pz,       // posición
        1.0f, 1.0f, 1.0f, // color blanco
        tu, tv,            // textura
        nx, ny, nz);       // normal

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