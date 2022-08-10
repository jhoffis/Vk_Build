#pragma once

#include <memory>
#include "src/window.h"

namespace Gra {
    std::shared_ptr<VkPhysicalDevice> pickPhysicalDevice(VkInstance& instance);
}
