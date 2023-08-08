#pragma once

#include <cstdint>
#include "src/window.h"
#include <vector>

namespace Gra {
    extern VkDescriptorSetLayout m_descriptorSetLayout;
    extern std::vector<VkDescriptorSet> m_descriptorSets;
    extern VkDescriptorPool m_descriptorPool;

    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage, float x, uint32_t offset);
    VkDescriptorSetLayout createDescriptorSetLayout();

    VkDescriptorPool createDescriptorPool();
    std::vector<VkDescriptorSet> createDescriptorSets(VkDescriptorPool pool);
    void cleanupUniform();
}