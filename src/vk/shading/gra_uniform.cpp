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

namespace Gra {

    StandardUBOMem createUniformBuffers(int amount) {
        // TODO make custom shit, hmmm vel du kan basere deg på en size av en struct som har endret størrelse da! Lag en struct med en vector med components kanskje?
        auto singleSize = sizeof(UniformBufferObject);
        auto offset = singleSize >= m_deviceProperties.limits.minUniformBufferOffsetAlignment ? singleSize : m_deviceProperties.limits.minUniformBufferOffsetAlignment;
        VkDeviceSize bufferSize = (amount-1)*offset+ 2*amount*singleSize;

        std::cout << "created ubo with Range " << singleSize << " and offset " << offset << std::endl;

        StandardUBOMem uboMem{};
        uboMem.size = amount;
        uboMem.range = static_cast<int>(singleSize);
        uboMem.offset = static_cast<int>(offset);
        uboMem.uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        uboMem.uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            createBuffer(bufferSize,
                         VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         uboMem.uniformBuffers[i],
                         uboMem.uniformBuffersMemory[i]
            );
        }
        return uboMem;
    }


    void updateUniformBuffer(StandardUBOMem uboMem, uint32_t currentSwapImage, uint32_t entityIndex, Entity *entity) {

        UniformBufferObject ubo{};
        ubo.model = glm::mat4(1.0f); //glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(x, x, 1.0f));
        ubo.model = glm::translate(ubo.model,
                                   glm::vec3(
                                           -(entity->pos.x - Camera::m_cam.x),
                                           -(entity->pos.y - Camera::m_cam.y),
                                           -entity->pos.z));

        ubo.view = glm::lookAt(glm::vec3(0, .00000000000000001f, 1), glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f));

        auto aspect = (float) m_swapChainExtent.width / (float) m_swapChainExtent.height;

        ubo.proj = glm::ortho(0.0f, aspect, 1.0f, 0.0f, -10.0f, 10.0f);

//        ubo.proj = glm::perspective(glm::radians(80.0f), (float) m_swapChainExtent.width / (float) m_swapChainExtent.height,
//                                    0.1f, 10.0f);
//        ubo.proj[1][1] *= -1; // GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted.

        void *data;
        vkMapMemory(m_device,
                    uboMem.uniformBuffersMemory[currentSwapImage],
                    entityIndex * uboMem.offset,
                    uboMem.range,
                    0,
                    &data);
        memcpy(data,
               &ubo,
               uboMem.range);
        vkUnmapMemory(m_device,
                      uboMem.uniformBuffersMemory[currentSwapImage]);
    }


    VkDescriptorSetLayout createDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr; // only relevant for image sampling related descriptors

        // This descriptor makes it possible for shaders to access an image resource through a sampler object
        VkDescriptorSetLayoutBinding samplerLayoutBinding{
                .binding = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount = 1,
                .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, // fragment shader.  It is possible to use texture sampling in the vertex shader, for example to dynamically deform a grid of vertices by a heightmap
                .pImmutableSamplers = nullptr,
        };

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        VkDescriptorSetLayout descriptorSetLayout{};
        if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
        return descriptorSetLayout;
    }

    VkDescriptorPool createDescriptorPool(int amountEntities) {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = amountEntities*2; //static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); // WHYYYYY
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = amountEntities*2; //static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = amountEntities * 3; //static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        VkDescriptorPool pool{};
        if (vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
        return pool;
    }

    std::vector<VkDescriptorSet> createDescriptorSets(VkDescriptorSetLayout &layout,
                                                      VkDescriptorPool &pool,
                                                      StandardUBOMem &uboMem,
                                                      VkImageView &textureImageView) {
        auto size = MAX_FRAMES_IN_FLIGHT * uboMem.size;
        std::vector<VkDescriptorSetLayout> layouts(size, layout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = pool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(size);
        allocInfo.pSetLayouts = layouts.data();

        std::vector<VkDescriptorSet> descriptorSets{};
        descriptorSets.resize(size);
        auto res = vkAllocateDescriptorSets(m_device, &allocInfo, descriptorSets.data());
        if (res != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = textureImageView;
        imageInfo.sampler = Texture::m_textureSampler;

        for (auto i = 0; i < size; i++) {

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uboMem.uniformBuffers[i % MAX_FRAMES_IN_FLIGHT]; // TODO Her er bindingen til ubo o.l.
            bufferInfo.offset = uboMem.offset * static_cast<int>(std::floor(
                    static_cast<float>(i) / static_cast<float>(MAX_FRAMES_IN_FLIGHT)));
            bufferInfo.range = uboMem.range;

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0,
                                   nullptr);
        }

        return descriptorSets;
    }

    void cleanupUniform() {
    }

} // Gra