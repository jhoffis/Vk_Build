#pragma once

#include <vector>
#include "src/window.h"
#include "gra/Mesh.h"
#include "vk/pipeline/gra_pipeline.h"
//#include "vk/shading/gra_vertex.h"

namespace Gra {

    struct CmdBuffer {
        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers; // vector for swap-chain
    };

    const int MAX_FRAMES_IN_FLIGHT = 2;

    extern VkCommandPool m_commandPool;
    extern std::vector<std::vector<VkCommandBuffer>> m_commandBuffers;

    void createCommandPool();
    void createCommandBuffers();
    void recordCommandBuffer(
            VkCommandBuffer commandBuffer,
            uint32_t imageIndex,
            const Mesh& mesh,
            const Raster::Pipeline& pipe,
            VkDescriptorSet *descriptorSet
            );

}