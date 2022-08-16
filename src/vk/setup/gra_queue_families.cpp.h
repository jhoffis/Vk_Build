#pragma once

#include "src/window.h"
#include <cstdint>
#include <optional>
#include <memory>


struct QueueFamilyIndices {
    // C++17 introduced a data structure to distinguish between the case of a value existing or not:
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

namespace Gra {
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
}
