#pragma once

#include <cstdint>
#include "window.h"
#include "math/mat.h"
#include <vector>
#include "vk/shading/texture.h"

namespace Gra {

//    extern std::vector<VkDescriptorSet> m_descriptorSets;

    struct Uniform {
        std::vector<VkBuffer> uniformBuffers{}; // Buffer and buffersizes
        std::vector<VkDeviceMemory> uniformBuffersMemory{}; // actual memory allocated on the vram

        VkDescriptorSetLayout descriptorSetLayout{};
        std::vector<VkDescriptorSet> descriptorSets{};
        VkDescriptorPool descriptorPool{};
    };

    struct UniformBufferObject {
        alignas(16) Math::Mat model;
        alignas(16) Math::Mat view;
        alignas(16) Math::Mat proj;
    };

    void createUniformBuffers(Uniform &uniform);
    void updateUniformBuffer(Uniform &uniform, uint32_t currentImage);

    void createDescriptorSetLayout(Uniform& uniform);
    void createDescriptorPool(Uniform& uniform);
    void createDescriptorSets(const Texture::TexData &tex, Uniform& uniform);
    void cleanupUniform(const Uniform &uniform);
}
