//
// Created by jonhof on 8/14/2023.
//

#ifndef VULKAN_MESH2D_H
#define VULKAN_MESH2D_H


#include <cstdint>
#include <vulkan/vulkan.h>
#include <vector>
#include "Vertex.h"

struct Mesh2D {
    VkBuffer vertexBuffer{};
    VkBuffer indexBuffer{};
    const uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
     //TODO bare om denne variabelen avkommenteres så kommer den til å bare tegne EN sprite!
    const Vertex vertices[4] =
            {
                    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                    {{0.5f,  -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                    {{0.5f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                    {{-0.5f, 0.5f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
            };
};
extern Mesh2D m_mesh;

#endif //VULKAN_MESH2D_H
