#pragma once

#include "src/window.h"

namespace Gra {
    extern VkRenderPass m_renderPass;

    void createRenderPass();
    void beginRenderPass(VkCommandBuffer commandBuffer, uint32_t imageIndex);
}