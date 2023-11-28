#pragma once

#include <cstdint>
#include "window.h"
#include "gra_elems/Entity.h"
#include "vk/gra_setup.h"
#include <vector>

namespace Gra {

    struct StandardUBOMem {
        int size{};
        int offset{};
        int range{};
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
    void updateUniformBuffer(StandardUBOMem uboMem, uint32_t currentSwapImage, uint32_t entityIndex, Entity *entity);
    VkDescriptorSetLayout createDescriptorSetLayout();

    VkDescriptorPool createDescriptorPool(int amountEntities);
    std::vector<VkDescriptorSet> createDescriptorSets(VkDescriptorSetLayout &layout,
                                                      VkDescriptorPool &pool,
                                                      StandardUBOMem &uboMem,
                                                      VkImageView &textureImageView);
    void cleanupUniform();
}