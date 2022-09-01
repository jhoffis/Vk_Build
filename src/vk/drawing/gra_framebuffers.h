#pragma once

#include "src/window.h"
#include <vector>

namespace Gra {

    extern std::vector<VkFramebuffer> m_swapChainFramebuffers;

    void createFramebuffers();
}