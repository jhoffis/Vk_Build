#pragma once

#include <cstdint>
#include <vulkan/vulkan.h>
#include <vector>
#include "Vertex.h"

struct Mesh2D {
    VkBuffer vertexBuffer{};
    VkBuffer indexBuffer{};
    const uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
    Vertex vertices[4];

    void init(float width, float height);
};
