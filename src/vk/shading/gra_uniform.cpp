//
// Created by Jens Benz on 08.10.2022.
//
#include <chrono>

#include "gra_uniform.h"
#include "gra_vertex.h"
#include "src/vk/drawing/gra_command_buffers.h"
#include "gra_memory_utils.h"
#include "src/vk/presentation/gra_swap_chain.h"
#include "src/vk/gra_setup.h"
#include "vk/setup/gra_physical_device.h"
#include "camera.h"
#include <cmath>
#include <iostream>
#include <cstring>

namespace Gra_Uniform {

    UBOMem createUniformBuffers(int amount, int sizeOfUBO) {
        // TODO make custom shit, hmmm vel du kan basere deg på en size av en struct som har endret størrelse da! Lag en struct med en vector med components kanskje?
        int singleSize = sizeOfUBO; // sizeof(UniformBufferObject);
        int offset;
        if (singleSize <= Gra::m_deviceProperties.limits.minUniformBufferOffsetAlignment) {
           offset = static_cast<int>(Gra::m_deviceProperties.limits.minUniformBufferOffsetAlignment);
        } else {
           int multiple = std::ceil(static_cast<float>(singleSize) /  static_cast<float>(Gra::m_deviceProperties.limits.minUniformBufferOffsetAlignment));
           offset = multiple * static_cast<int>(Gra::m_deviceProperties.limits.minUniformBufferOffsetAlignment);
        }
        VkDeviceSize bufferSize = amount * offset + 1;

        std::cout << "created ubo with Range " << singleSize << " and offset " << offset << std::endl;

        UBOMem uboMem{};
        uboMem.amount = amount;
        uboMem.range = singleSize;
        uboMem.offset = offset;
        uboMem.uniformBuffers.resize(Gra::MAX_FRAMES_IN_FLIGHT);
        uboMem.uniformBuffersMemory.resize(Gra::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < Gra::MAX_FRAMES_IN_FLIGHT; i++) {
            Gra::createBuffer(bufferSize,
                              VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                              uboMem.uniformBuffers[i],
                              uboMem.uniformBuffersMemory[i]
            );
        }
        return uboMem;
    }

    // this is how one copies memory to a uniform?
    void updateUniformBuffer(const UBOMem &uboMem,
                             const uint32_t currentSwapImage,
                             const uint32_t entityIndex) {
        void *data;
        vkMapMemory(Gra::m_device,
                    uboMem.uniformBuffersMemory[currentSwapImage],
                    entityIndex * uboMem.offset,
                    uboMem.range,
                    0,
                    &data);
        if (uboMem.uboStruct == nullptr) {
            memset(data, 0, uboMem.range);
        } else {
            memcpy(data,
                   uboMem.uboStruct,
                   uboMem.range);
        }
        vkUnmapMemory(Gra::m_device,
                      uboMem.uniformBuffersMemory[currentSwapImage]);
    }

    void clearUniformBuffer(const UBOMem &uboMem,
                            const uint32_t currentSwapImage,
                            const uint32_t startEntityIndex) {
        void *data;
        vkMapMemory(Gra::m_device,
                    uboMem.uniformBuffersMemory[currentSwapImage],
                    startEntityIndex * uboMem.offset,
                    (uboMem.amount - startEntityIndex) * uboMem.offset,
                    0,
                    &data);
        // Calculate the size of the memory to clear
        size_t clearSize = (uboMem.amount - startEntityIndex) * uboMem.offset;
        // Clear the memory to zero
        memset(data, 0, clearSize);
        vkUnmapMemory(Gra::m_device,
                      uboMem.uniformBuffersMemory[currentSwapImage]);
    }


    VkDescriptorSetLayout createDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings) {
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        VkDescriptorSetLayout descriptorSetLayout{};
        if (vkCreateDescriptorSetLayout(Gra::m_device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
        return descriptorSetLayout;
    }

    VkDescriptorPool createDescriptorPool(int amountEntities) {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = amountEntities * 2; //static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); // WHYYYYY
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = amountEntities * 2; //static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = amountEntities * 3; //static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        VkDescriptorPool pool{};
        if (vkCreateDescriptorPool(Gra::m_device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
        return pool;
    }

} // Gra