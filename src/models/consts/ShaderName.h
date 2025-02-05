#pragma once

#include <vulkan/vulkan.h>
#include <string>

enum ShaderName {
    grass, selectionBox
};

enum ShaderComponentOrder {
    vert_ubo, frag_image
};

// Yes. This is related to the filename
std::string getShaderName(const ShaderName &shader);

namespace UBOComponent {
    VkDescriptorSetLayoutBinding binding(uint32_t binding);
}

namespace ImageComponent {
    VkDescriptorSetLayoutBinding binding(uint32_t binding);
}
