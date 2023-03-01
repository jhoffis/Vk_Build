#pragma once
#include "src/window.h"

namespace Gra {
    VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice *physicalDevice);
    void setMaxUsableSampleCount(VkPhysicalDevice *physicalDevice);
}