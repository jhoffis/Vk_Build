//
// Created by jonhof on 8/14/2023.
//

#ifndef VULKAN_MESH_H
#define VULKAN_MESH_H


#include <cstdint>
#include <vulkan/vulkan.h>
#include <vector>
#include "vk/gra_setup.h"
#include "vk/shading/gra_vertex.h"
#include <memory>

namespace Mesh {
    struct Mesh {
        VkBuffer vertexBuffer{};
        VkBuffer indexBuffer{};
        const std::array<uint32_t, 6> indices {0, 1, 2, 2, 3, 0};
        std::array<Gra::Vertex, 4> vertices{};

        void destroy() {
            vkDestroyBuffer(Gra::m_device, indexBuffer, nullptr);
            vkDestroyBuffer(Gra::m_device, vertexBuffer, nullptr);
        }
    };

    void init(Mesh *mesh);

    void cleanup();
}
#endif //VULKAN_MESH_H
