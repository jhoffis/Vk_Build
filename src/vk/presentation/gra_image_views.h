#pragma once

#include <vector>
#include "src/vk/gra_setup.h"

namespace Gra {
    extern std::vector<VkImageView> m_swapChainImageViews;

    void createImageViews();
}