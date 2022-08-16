#pragma once

#include <memory>
#include "src/window.h"

namespace Gra {
    std::shared_ptr<VkPhysicalDevice> pickPhysicalDevice(std::shared_ptr<VkInstance>& instance, std::shared_ptr<VkSurfaceKHR> &surface);
}
