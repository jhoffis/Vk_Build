#pragma once
#include "src/window.h"

namespace Gra {

    extern VkImageView colorImageView;
    extern VkSampleCountFlagBits msaaSamples;

    VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice *physicalDevice);
    void setMaxUsableSampleCount(VkPhysicalDevice *physicalDevice);
    void createColorResources();
    void destroyColorResources(VkDevice device);
}