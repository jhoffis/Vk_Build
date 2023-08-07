#pragma once

#include <cstdint>
#include "src/window.h"
#include <vector>

namespace Gra {
    extern VkDescriptorSetLayout m_descriptorSetLayout;
    extern std::vector<VkDescriptorSet> m_descriptorSets;

    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage, float x, uint32_t offset);
    VkDescriptorSetLayout createDescriptorSetLayout();

    void createDescriptorPool();
    void createDescriptorSets();
    void cleanupUniform();
}