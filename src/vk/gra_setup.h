#pragma once

#include <memory>
#include "src/window.h"
#include "vk/shading/model.h"
#include "vk/shading/texture.h"

namespace Gra {

    extern VkDevice m_device;
    extern std::shared_ptr<VkSurfaceKHR> m_surface;
    extern std::shared_ptr<VkPhysicalDevice> m_physicalDevice;


    void initVulkan();
    void initRest(Texture::TexData tex);
    void cleanup();

}