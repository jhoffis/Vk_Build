#pragma once

#include <vulkan/vulkan.h>

enum ShaderName {
    grass, selectionBox
};

namespace UBOComponent {
    VkDescriptorSetLayoutBinding binding(uint32_t binding);
}

namespace ImageComponent {
    VkDescriptorSetLayoutBinding binding(uint32_t binding);
    VkDescriptorImageInfo imageInfo()
}
