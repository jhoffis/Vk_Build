#pragma once

#include "src/window.h"

namespace Gra {
    std::shared_ptr<VkInstance> createInstance(bool enableValidationLayers,
                                               const std::vector<const char *>& validationLayers);
}