#pragma once

#include "src/window.h"

namespace Gra {

    extern VkCommandPool m_commandPool;
    extern VkCommandBuffer m_commandBuffer;

    void createCommandPool();
    void createCommandBuffer();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

}