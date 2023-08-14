//
// Created by jonhof on 8/14/2023.
//

#ifndef VULKAN_MESH_H
#define VULKAN_MESH_H


#include <cstdint>
#include <vulkan/vulkan.h>
#include <vector>

struct Mesh {
    VkBuffer vertexBuffer{};
    VkBuffer indexBuffer{};
    std::vector<uint32_t> indices{};
};
extern Mesh m_mesh;

#endif //VULKAN_MESH_H
