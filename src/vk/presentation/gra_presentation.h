#pragma once
#include "src/window.h"
#include <memory>

namespace Gra {
    VkSurfaceKHR createSurface(std::shared_ptr<VkInstance>& instance);
};
