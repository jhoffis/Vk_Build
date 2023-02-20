//
// Created by jh on 8/10/2022.
//
#include "gra_physical_device.h"
#include "gra_queue_families.cpp.h"
#include "src/vk/presentation/gra_swap_chain.h"
#include <stdexcept>
#include <vector>
#include <map>
#include <iostream>
#include <set>


bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(Gra::deviceExtensions.begin(), Gra::deviceExtensions.end());

    for (const auto &extension: availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


bool isDeviceSuitable(VkPhysicalDevice device) {
    auto indices = Gra::findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = Gra::querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}


auto rateDeviceSuitability(
        VkPhysicalDevice device,
        std::shared_ptr<VkSurfaceKHR> &surface
) -> uint32_t {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    uint32_t score = 0;
    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    QueueFamilyIndices indices = Gra::findQueueFamilies(device);
    // Application can't function without geometry shaders
    if (!deviceFeatures.geometryShader || !indices.isComplete() || !isDeviceSuitable(device))
        return 0;
    return score;
}


auto Gra::pickPhysicalDevice(
        std::shared_ptr<VkInstance> &instance,
        std::shared_ptr<VkSurfaceKHR> &surface
) -> std::shared_ptr<VkPhysicalDevice> {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto &device: devices) {
        auto score = rateDeviceSuitability(device, surface);
        candidates.insert(std::make_pair(score, device));
    }

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        auto physicalDevice = candidates.rbegin()->second;
        // print name of the device of choice:
        VkPhysicalDeviceProperties2 properties{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
        };
        vkGetPhysicalDeviceProperties2(physicalDevice, &properties);
        std::cout << "Picked GPU: " << properties.properties.deviceName << std::endl;

        return std::make_shared<VkPhysicalDevice>(physicalDevice);
    } else {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}
