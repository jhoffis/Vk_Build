#pragma once
#include <vector>
#include "gra_vertex.h"

namespace Model {

    struct Mesh {
        std::vector<Gra::Vertex> vertices;
        std::vector<uint32_t> indices;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
    };

    Mesh* loadModel(const char *name);
}