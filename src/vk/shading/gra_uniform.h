#pragma once

#include <cstdint>
#include "src/window.h"
#include "gra/Entity.h"
#include "vk/gra_setup.h"
#include <vector>

namespace Gra {
    extern std::vector<VkDescriptorSet> m_descriptorSets;
    extern VkDescriptorPool m_descriptorPool;

    struct StandardUBOMem {
        int size{};
        std::vector<VkBuffer> uniformBuffers{};
        std::vector<VkDeviceMemory> uniformBuffersMemory{};

        void destroy() {
            for (size_t i = 0; i < uniformBuffers.size(); i++) {
                vkDestroyBuffer(m_device, uniformBuffers[i], nullptr);
                vkFreeMemory(m_device, uniformBuffersMemory[i], nullptr);
            }
        }
    };

    StandardUBOMem createUniformBuffers(int amount);
    void updateUniformBuffer(StandardUBOMem uboMem, uint32_t currentImage, uint32_t offset, Entity &entity);
    VkDescriptorSetLayout createDescriptorSetLayout();

    VkDescriptorPool createDescriptorPool();
    std::vector<VkDescriptorSet> createDescriptorSets(VkDescriptorSetLayout &layout,
                                                      VkDescriptorPool &pool,
                                                      StandardUBOMem &uboMem,
                                                      VkImageView &textureImageView);
    void cleanupUniform();
}