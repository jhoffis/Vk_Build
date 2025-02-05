//
// Created by jh on 8/10/2022.
//
#include "gra_logical_device.h"
#include "gra_queue_families.cpp.h"
#include "src/vk/setup/gra_physical_device.h"
#include <vector>
#include <stdexcept>
#include <set>
#include <cstring>
#include <iostream>

namespace Gra {

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    VkDevice createLogicalDevice(
            bool enableValidationLayers,
            std::vector<const char *> &validationLayers,
            std::shared_ptr<VkPhysicalDevice> &physicalDevice
    ) {
        QueueFamilyIndices indices = findQueueFamilies(*physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily: uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .queueFamilyIndex = queueFamily,
                    .queueCount = 1,
                    .pQueuePriorities = &queuePriority,
            };
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{
            .samplerAnisotropy = VK_TRUE
        };

        VkDeviceCreateInfo createInfo{
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
                .pQueueCreateInfos = queueCreateInfos.data(),
                .enabledExtensionCount = static_cast<uint32_t>(Gra::deviceExtensions.size()),
                .ppEnabledExtensionNames = Gra::deviceExtensions.data(),
                .pEnabledFeatures = &deviceFeatures,
        };

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        VkDevice device;
        if (vkCreateDevice(*physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &m_presentQueue);

        return device;
    }


    bool checkValidationLayerSupport(std::vector<const char *> &validationLayers) {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (auto layerName: validationLayers) {
            bool layerFound = false;

            for (const auto &layerProperties: availableLayers) {
#ifdef RMDEV
                std::cout << "LayerName: " << layerName << " ?= Available layer: " << layerProperties.layerName << std::endl;
#endif
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }
        return true;
    }
}