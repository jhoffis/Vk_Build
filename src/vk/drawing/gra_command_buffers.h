#pragma once

#include <vector>
#include "src/window.h"

namespace Gra {

    const int MAX_FRAMES_IN_FLIGHT = 2;

    extern VkCommandPool m_commandPool;
    extern std::vector<VkCommandBuffer> m_commandBuffers;

    void createCommandPool();
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

}