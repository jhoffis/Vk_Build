#pragma once

#include <vector>
#include "vk/gra_descriptors.h"
#include "window.h"
#include "rendering/Mesh2D.h"
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
    void recordCommandBuffer(VkCommandBuffer commandBuffer,
                             uint32_t imageIndex,
                             const Mesh2D& mesh,
                             const Raster::Pipeline& pipe,
                             std::vector<Gra_desc::DescriptorSet> &descriptorSets,
                             uint32_t entityCount,
                             const Gra_Uniform::UBOMem uboMem);

}
