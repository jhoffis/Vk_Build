#pragma once
#include <vector>
// #include "gra_vertex.h"

namespace Model {

    // struct Mesh {
    //     std::vector<Gra::Vertex> Gvertices;
    //     std::vector<uint32_t> Gindices;
    //     VkBuffer vertexBuffer;
    //     VkDeviceMemory vertexBufferMemory;
    // };

    void loadModel(const char *name);
}