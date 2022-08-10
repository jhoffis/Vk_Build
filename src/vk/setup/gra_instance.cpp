//
// Created by jh on 8/10/2022.
//
#include <vector>
#include <stdexcept>
#include <memory>
#include "gra_instance.h"
#include "gra_logical_device.h"
#include "gra_debug.h"

VkInstance m_instance;


std::vector<const char *> getRequiredExtensions(
        bool enableValidationLayers
) {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}


std::shared_ptr<VkInstance> Gra::createInstance(
        bool enableValidationLayers,
        const std::vector<const char *>& validationLayers
) {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation Layers requested, but not available");
    }

    VkApplicationInfo appInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "Rm",
            .applicationVersion = VK_MAKE_VERSION(2, 0, 0),
            .pEngineName = "Rm Engine",
            .engineVersion = VK_MAKE_VERSION(2, 0, 0),
            .apiVersion = VK_API_VERSION_1_3,
    };

    auto extensions = getRequiredExtensions(enableValidationLayers);
    VkInstanceCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
    };

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }

    return std::make_shared<VkInstance>(m_instance);
}

