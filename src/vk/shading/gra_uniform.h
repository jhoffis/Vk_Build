#pragma once

#include <cstdint>
#include "window.h"
#include "math/mat.h"
#include <vector>
#include "vk/shading/texture.h"

namespace Gra {

    struct Uniform {
        VkDescriptorSetLayout descriptorSetLayout;
        std::vector<VkDescriptorSet> descriptorSets;
        VkDescriptorPool descriptorPool;
    };

    struct UniformBufferObject {
        alignas(16) Math::Mat model;
        alignas(16) Math::Mat view;
        alignas(16) Math::Mat proj;
    };

    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);

    void createDescriptorSetLayout(Uniform& uniform);
    void createDescriptorPool();
    void createDescriptorSets(Texture::TexData, Uniform& uniform);
    void cleanupUniform();
}
