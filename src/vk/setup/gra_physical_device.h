#pragma once

#include <memory>
#include <vector>
#include "src/window.h"

namespace Gra {

    const std::vector<const char *> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    std::shared_ptr<VkPhysicalDevice> pickPhysicalDevice(std::shared_ptr<VkInstance>& instance, std::shared_ptr<VkSurfaceKHR> &surface);
}
