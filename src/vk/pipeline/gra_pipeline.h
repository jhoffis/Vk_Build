#pragma once

#include "src/window.h"

namespace Raster {

    struct Pipeline {
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
    };

    extern Pipeline m_pipeline;

    Pipeline createGraphicsPipeline(VkDescriptorSetLayout descriptorSetLayout, const std::string &shaderName);

    void destroyPipeline(Pipeline &pipeline);
}