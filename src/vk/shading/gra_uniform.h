#pragma once

#include <cstdint>
#include "window.h"
#include "gra_elems/Entity.h"
#include "vk/gra_setup.h"
#include "gra_elems/ShaderName.h"
#include <vector>

namespace Gra_Uniform {

    struct UBOMem {
        int amount{};
        int offset{};
        int range{};
        void *uboStruct{};
        std::vector<VkBuffer> uniformBuffers{};
        std::vector<VkDeviceMemory> uniformBuffersMemory{};

        void destroy() {
            for (size_t i = 0; i < uniformBuffers.size(); i++) {
                vkDestroyBuffer(Gra::m_device, uniformBuffers[i], nullptr);
                vkFreeMemory(Gra::m_device, uniformBuffersMemory[i], nullptr);
            }
        }

    };

    UBOMem createUniformBuffers(int amount, int sizeOfUBO);
    void updateUniformBuffer(const UBOMem &uboMem,
                             uint32_t currentSwapImage,
                             uint32_t entityIndex);
    VkDescriptorSetLayout createDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings);

    VkDescriptorPool createDescriptorPool(int amountEntities);
    std::vector<VkDescriptorSet> createDescriptorSets(const ShaderName &shader,
                                                      VkDescriptorSetLayout &layout,
                                                      VkDescriptorPool &pool,
                                                      UBOMem &uboMem,
                                                      VkImageView &textureImageView);
}