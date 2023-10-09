//
// Created by jh on 8/12/2022.
//
#include <set>
#include <algorithm>
#include "gra_swap_chain.h"
#include "src/vk/gra_setup.h"
#include "src/vk/setup/gra_queue_families.cpp.h"
#include "gra_image_views.h"
#include "src/vk/drawing/gra_framebuffers.h"
#include "src/vk/shading/gra_depth.h"
#include "src/file_util.h"
#include "src/window.h"

VkSwapchainKHR Gra::m_swapChain;
std::vector<VkImage> Gra::m_swapChainImages;
VkFormat Gra::m_swapChainImageFormat;
VkExtent2D Gra::m_swapChainExtent;

SwapChainSupportDetails Gra::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;
    // Capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *m_surface, &details.capabilities);
    // Formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, *m_surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, *m_surface, &formatCount, details.formats.data());
    }
    // PresentModes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, *m_surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, *m_surface, &presentModeCount,
                                                  details.presentModes.data());
    }
    return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    for (const auto &availableFormat: availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
    for (const auto &availablePresentMode: availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(Window::m_window, &width, &height);

        VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                        capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                                         capabilities.maxImageExtent.height);

        return actualExtent;
    }
}


void Gra::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(*Gra::m_physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = *Gra::m_surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(*Gra::m_physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    // we only specified a minimum number of images in the swap chain,
    // so the implementation is allowed to create a swap chain with more.
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}

void Gra::cleanupSwapChain() {
    destroyDepth(m_device);

    for (auto & m_swapChainFramebuffer : m_swapChainFramebuffers) {
        vkDestroyFramebuffer(m_device, m_swapChainFramebuffer, nullptr);
    }

    for (auto & m_swapChainImageView : m_swapChainImageViews) {
        vkDestroyImageView(m_device, m_swapChainImageView, nullptr);
    }

    vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
}

void Gra::recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(Window::m_window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(Window::m_window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_device);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createDepthResources();
    createFramebuffers();
}

void Gra::recordSwapChain(VkCommandBuffer commandBuffer) {
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_swapChainExtent.width);
    viewport.height = static_cast<float>(m_swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}
