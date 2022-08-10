//
// Created by jh on 8/10/2022.
//
#include "gra_physical_device.h"
#include "gra_queue_families.cpp.h"
#include <stdexcept>
#include <vector>
#include <map>
#include <iostream>


auto rateDeviceSuitability(VkPhysicalDevice device) -> uint32_t {
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
    if (!deviceFeatures.geometryShader || !indices.isComplete())
        return 0;
    return score;
}


auto Gra::pickPhysicalDevice(VkInstance& instance) -> std::shared_ptr<VkPhysicalDevice> {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) {
        auto score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        auto physicalDevice = candidates.rbegin()->second;
        // print name of the device of choice:
        VkPhysicalDeviceProperties2 properties{};
        vkGetPhysicalDeviceProperties2(physicalDevice, &properties);
        std::cout << "Picked GPU: " << properties.properties.deviceName << std::endl;

        return std::make_shared<VkPhysicalDevice>(physicalDevice);
    } else {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}
