#pragma once

#include "src/window.h"
#include <cstdint>
#include <optional>

struct QueueFamilyIndices {
    // C++17 introduced a data structure to distinguish between the case of a value existing or not:
    std::optional<uint32_t> graphicsFamily;

    [[nodiscard]] constexpr bool isComplete() const {
        return graphicsFamily.has_value();
    }
};

namespace Gra {
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
}
