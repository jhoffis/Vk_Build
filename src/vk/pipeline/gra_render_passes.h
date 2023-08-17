#pragma once

#include "src/window.h"

namespace Gra {
    extern VkRenderPass m_renderPass;

    VkRenderPass createRenderPass(bool clear);
    void createRenderPasses();
    void beginRenderPass(VkCommandBuffer commandBuffer, uint32_t imageIndex, bool clear);
    void destroyRenderPasses();
}