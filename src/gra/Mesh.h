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

namespace Mesh {
    struct Mesh {
        VkBuffer vertexBuffer{};
        VkBuffer indexBuffer{};
        std::vector<uint32_t> indices{};
        std::vector<Gra::Vertex> vertices{};

        void destroy() {
            vkDestroyBuffer(Gra::m_device, indexBuffer, nullptr);
            vkDestroyBuffer(Gra::m_device, vertexBuffer, nullptr);
        }
    };

    void init(Mesh *mesh);

    void cleanup();
}
#endif //VULKAN_MESH_H
