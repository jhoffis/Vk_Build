#pragma once

#include <memory>
#include <vector>
#include "src/window.h"

namespace Gra {

    extern VkPhysicalDeviceProperties m_deviceProperties;

    const std::vector<const char *> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    std::shared_ptr<VkPhysicalDevice> pickPhysicalDevice(std::shared_ptr<VkInstance>& instance, std::shared_ptr<VkSurfaceKHR> &surface);
}
