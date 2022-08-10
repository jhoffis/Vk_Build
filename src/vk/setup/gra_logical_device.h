#pragma once

#include <vector>
#include "src/window.h"

namespace Gra {
    void createLogicalDevice(bool enableValidationLayers,
                             VkPhysicalDevice& physicalDevice,
                             std::vector<const char *>& validationLayers);
    bool checkValidationLayerSupport(std::vector<const char *>& validationLayers);
}
