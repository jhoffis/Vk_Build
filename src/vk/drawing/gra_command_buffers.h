#pragma once

#include <vector>
#include "window.h"
#include "vk/shading/model.h"

namespace Gra {

    const int MAX_FRAMES_IN_FLIGHT = 2;

    extern VkCommandPool m_commandPool;
    extern std::vector<VkCommandBuffer> m_commandBuffers;

    void createCommandPool();
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, Model::Mesh mesh);

}