#pragma once

#include <cstdint>
#include "src/window.h"
#include "gra/Entity.h"
#include <vector>

namespace Gra {
    extern std::vector<VkDescriptorSet> m_descriptorSets;
    extern VkDescriptorPool m_descriptorPool;

    struct StandardUBOMem {
        std::vector<VkBuffer> uniformBuffers{};
        std::vector<VkDeviceMemory> uniformBuffersMemory{};
    };

    StandardUBOMem createUniformBuffers();
    void updateUniformBuffer(StandardUBOMem uboMem, uint32_t currentImage, uint32_t offset, Entity &entity);
    VkDescriptorSetLayout createDescriptorSetLayout();

    VkDescriptorPool createDescriptorPool();
    std::vector<VkDescriptorSet> createDescriptorSets(VkDescriptorSetLayout layout,
                                                      VkDescriptorPool pool,
                                                      StandardUBOMem uboMem,
                                                      VkImageView textureImageView);
    void cleanupUniform();
}