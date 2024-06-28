#pragma once

#include "math/Vec2.h"
#include "math/Vec3.h"
#include "src/window.h"
#include <array>
#include <vulkan/vulkan_core.h>

struct InstanceData {
  Vec2 pos{};
  uint32_t index{};
};

struct Vertex {
  Vec3 pos;
  Vec3 color;
  Vec2 texCoord;

  static std::array<VkVertexInputBindingDescription, 2>
  getBindingDescription() {
    std::array<VkVertexInputBindingDescription, 2> bindingDescs{};
    VkVertexInputBindingDescription bindingDescription{
        .binding = 0,
        .stride = sizeof(Vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX};
    VkVertexInputBindingDescription bindingDescription2{
        .binding = 1,
        .stride = sizeof(InstanceData),
        .inputRate = VK_VERTEX_INPUT_RATE_INSTANCE};
    bindingDescs[0] = bindingDescription;
    bindingDescs[1] = bindingDescription2;

    return bindingDescs;
  }

  static std::array<VkVertexInputAttributeDescription, 5>
  getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    attributeDescriptions[3].binding = 1;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[3].offset = offsetof(InstanceData, pos);

    attributeDescriptions[4].binding = 1;
    attributeDescriptions[4].location = 4;
    attributeDescriptions[4].format = VK_FORMAT_R32_UINT;
    attributeDescriptions[4].offset = offsetof(InstanceData, index);

    return attributeDescriptions;
  }
};
