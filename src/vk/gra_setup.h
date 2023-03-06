#pragma once

#include <memory>
#include "src/window.h"
#include "vk/shading/model.h"

namespace Gra {

    extern Model::Mesh mesh;
    extern VkDevice m_device;
    extern std::shared_ptr<VkSurfaceKHR> m_surface;
    extern std::shared_ptr<VkPhysicalDevice> m_physicalDevice;


    void initVulkan();
    void cleanup();

}