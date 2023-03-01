#pragma once

#include "window.h"

namespace Gra {

    extern VkImageView depthImageView;

    VkFormat findDepthFormat();
    void createDepthResources();
    void destroyDepth(VkDevice device);
}