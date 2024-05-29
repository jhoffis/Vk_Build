#include "ShaderName.h"

namespace UBOComponent {
    VkDescriptorSetLayoutBinding binding(uint32_t binding) {
        return {
                .binding = binding,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 1,
                .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr // only relevant for image sampling related descriptor,
        };
    }
}
namespace ImageComponent {
    VkDescriptorSetLayoutBinding binding(uint32_t binding) {
        return {
                .binding = binding,
                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount = 1,
                .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, // fragment shader.  It is possible to use texture sampling in the vertex shader, for example to dynamically deform a grid of vertices by a heightmap
                .pImmutableSamplers = nullptr,
        };
    }

}