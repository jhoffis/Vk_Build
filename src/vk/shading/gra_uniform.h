#pragma once

#include <cstdint>
#include "window.h"
#include "rendering/Entity.h"
#include "vk/gra_setup.h"
#include "models/consts/ShaderName.h"
#include "gra_vertex.h"
#include <vector>

namespace Gra_Uniform {

    struct UBOMem {
        int amount{}; // number of allocations after each other. Division of the buffer.
        int offset{}; // distance between each allocated n mem. Not less than minUniformBufferOffsetAlignment.
        int range{};  // bytes actually allocated
        void *uboStruct{}; // the data the buffer is filled with.
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
}