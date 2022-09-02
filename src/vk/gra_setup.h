#pragma once

#include <memory>
#include "src/window.h"

namespace Gra {

    extern VkDevice m_device;
    extern std::shared_ptr<VkSurfaceKHR> m_surface;
    extern std::shared_ptr<VkPhysicalDevice> m_physicalDevice;


    void initVulkan();
    void cleanup();

}