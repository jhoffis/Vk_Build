#pragma once

#include <memory>
#include "src/window.h"

namespace Gra {

    extern std::shared_ptr<VkSurfaceKHR> m_surface;

    void initVulkan();
    void cleanup();

}