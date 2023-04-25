#pragma once

#include <cstdint>
#include "window.h"
#include "math/mat.h"
#include <vector>
#include "vk/shading/texture.h"

namespace Gra {
    extern VkDescriptorSetLayout m_descriptorSetLayout;
    extern std::vector<VkDescriptorSet> m_descriptorSets;

    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);
    void createDescriptorSetLayout();

    void createDescriptorPool();
    void createDescriptorSets(Texture::TexData);
    void cleanupUniform();
}
