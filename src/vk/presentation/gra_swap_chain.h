#pragma once
#include "src/window.h"

#include <vector>
#include <memory>

const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace Gra {

    extern VkSwapchainKHR m_swapChain;
    extern std::vector<VkImage> m_swapChainImages;
    extern VkFormat m_swapChainImageFormat;
    extern VkExtent2D m_swapChainExtent;

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    void createSwapChain();
}