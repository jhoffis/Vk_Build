#pragma once
#include "src/window.h"
#include <memory>

namespace Gra {
    std::shared_ptr<VkSurfaceKHR> createSurface(std::shared_ptr<VkInstance>& instance);
};
