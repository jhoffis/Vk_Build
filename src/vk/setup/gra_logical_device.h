#pragma once

#include <vector>
#include <memory>
#include "src/window.h"

namespace Gra {
    VkDevice createLogicalDevice(bool enableValidationLayers,
                                 std::vector<const char *> &validationLayers,
                                 std::shared_ptr<VkPhysicalDevice> &physicalDevice
                                 );

    bool checkValidationLayerSupport(std::vector<const char *> &validationLayers);
}
