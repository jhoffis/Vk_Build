#pragma once

#include "src/window.h"
#include "vk/shading/gra_uniform.h"

namespace Gra {

    struct Pipeline {
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
    };

    Pipeline createGraphicsPipeline(const Gra::Uniform& uniform);
}