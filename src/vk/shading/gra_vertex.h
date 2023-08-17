#pragma once

#include "gra/Mesh2D.h"
#include "gra/Vertex.h"
#include <vector>

namespace Gra {

    const std::vector<Vertex> m_vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };
    // TODO lag en check på int størrelse i forhold til attributeDescriptions[1].format osv fordi om du definerer feil int størrelse så vil den ikke klage men heller ikke fungere!
    const std::vector<uint32_t> m_indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

//    struct Mesh2D {
//        VkBuffer *vertexBuffer;
//        VkBuffer *indexBuffer;
//        uint32_t *indices;
//    };

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    void createVertexBuffer(Mesh2D *mesh);
    void createIndexBuffer(Mesh2D *mesh);

    void cleanupVertex();
}
