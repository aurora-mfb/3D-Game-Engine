#pragma once

#include "../lib/glm/vec3.hpp"
#include "../lib/glm/vec2.hpp"

struct Vertex {
  glm::vec3 position;      // vpos (x, y, z)
  glm::vec3 color;         // vcolor (r, g, b)
  glm::vec2 textureCoord;  // vtex (u, v)
  glm::vec3 normal;        // vnormal (nx, ny, nz)

  Vertex()
    : position(0.0f), color(1.0f), textureCoord(0.0f), normal(0.0f) {
  }

  Vertex(float x, float y, float z,
    float r, float g, float b,
    float u, float v,
    float nx, float ny, float nz)
    : position(x, y, z), color(r, g, b), textureCoord(u, v), normal(nx, ny, nz) {
  }
};

