#pragma once

#include <cstdint>
#include "src/window.h"
#include <vector>

namespace Gra {
    extern VkDescriptorSetLayout m_descriptorSetLayout;
    extern std::vector<VkDescriptorSet> m_descriptorSets;
    extern VkDescriptorPool m_descriptorPool;

    struct StandardUBOMem {
        std::vector<VkBuffer> uniformBuffers{};
        std::vector<VkDeviceMemory> uniformBuffersMemory{};
    };

    StandardUBOMem createUniformBuffers();
    void updateUniformBuffer(StandardUBOMem uboMem, uint32_t currentImage, float x, uint32_t offset);
    VkDescriptorSetLayout createDescriptorSetLayout();

    VkDescriptorPool createDescriptorPool();
    std::vector<VkDescriptorSet> createDescriptorSets(VkDescriptorSetLayout layout, VkDescriptorPool pool, StandardUBOMem uboMem);
    void cleanupUniform();
}