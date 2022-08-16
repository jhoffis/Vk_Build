#pragma once

#include <memory>
#include "src/window.h"

namespace Gra {
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    void setupDebugMessenger(bool enableValidationLayers, std::shared_ptr<VkInstance>& instance);
    void cleanDebug(bool enableValidationLayers, VkInstance instance);
}