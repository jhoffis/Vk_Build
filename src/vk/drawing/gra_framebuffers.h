#pragma once

#include "src/window.h"

namespace Gra {

    extern std::vector<VkFramebuffer> m_swapChainFramebuffers;

    void createFramebuffers();
}