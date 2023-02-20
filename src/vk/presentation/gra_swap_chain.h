#pragma once
#include "src/window.h"

#include <vector>
#include <memory>

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
    void createSwapChain();
    void recreateSwapChain();
    void cleanupSwapChain();
}