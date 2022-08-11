#pragma once

#include "src/window.h"
#include <memory>
#include <vector>

namespace Gra {
    std::shared_ptr<VkInstance> createInstance(
            bool enableValidationLayers,
            std::vector<const char *>& validationLayers
    );
}