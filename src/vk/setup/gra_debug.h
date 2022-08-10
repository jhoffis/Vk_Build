#pragma once

#include "src/window.h"

namespace Gra {
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    void setupDebugMessenger(bool enableValidationLayers, VkInstance& instance);
}