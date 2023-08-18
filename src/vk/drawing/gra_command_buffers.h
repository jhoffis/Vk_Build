#pragma once

#include <vector>
#include "src/window.h"
#include "gra/Mesh2D.h"
#include "vk/pipeline/gra_pipeline.h"
//#include "vk/shading/gra_vertex.h"

namespace Gra {

    const int MAX_FRAMES_IN_FLIGHT = 2;

    struct CmdBuffer {
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers; // vector for swap-chain

        void init();
    };

    extern VkCommandPool m_commandPool;

    void createCommandPool();
    void recordCommandBuffer(
            VkCommandBuffer commandBuffer,
            uint32_t imageIndex,
            const Mesh2D& mesh,
            const Raster::Pipeline& pipe,
            std::vector<VkDescriptorSet> &descriptorSets
            );

}