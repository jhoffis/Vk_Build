#pragma once

#include "src/window.h"

namespace Gra {

    extern VkPipelineLayout m_pipelineLayout;
    extern VkPipeline m_graphicsPipeline;;

    void createGraphicsPipeline();
}