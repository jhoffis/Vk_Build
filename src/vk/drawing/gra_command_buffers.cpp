//
// Created by jh on 8/31/2022.
//
#include <stdexcept>

#include "gra_command_buffers.h"
#include "src/vk/setup/gra_queue_families.cpp.h"
#include "src/vk/gra_setup.h"
#include "src/vk/pipeline/gra_render_passes.h"
#include "gra_framebuffers.h"
#include "src/vk/presentation/gra_swap_chain.h"
#include "src/vk/pipeline/gra_pipeline.h"
#include "gra_drawing.h"
#include "src/vk/shading/gra_uniform.h"
#include "vk/shading/gra_vertex.h"


namespace Gra {

    VkCommandPool m_commandPool;
    std::vector<std::vector<VkCommandBuffer>> m_commandBuffers;

    void createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(*m_physicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    VkCommandPool createCommandPool2() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(*m_physicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        VkCommandPool pool{};
        if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
        return pool;
    }

    void createCommandBuffers() {
        m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        uint32_t size = 2;
        std::vector<VkCommandBuffer> tempCommandBuffers{2*size};

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) tempCommandBuffers.size();

        if (vkAllocateCommandBuffers(m_device, &allocInfo, tempCommandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            m_commandBuffers[i].resize(size);
            for (int a = 0; a < size; a++) {
                m_commandBuffers[i][a] = tempCommandBuffers[a + i*size];
            }
        }
    }

    std::vector<VkCommandBuffer> createCommandBuffers2() {
        std::vector<VkCommandBuffer> commandBuffers{};
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

        if (vkAllocateCommandBuffers(m_device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
        return commandBuffers;
    }

    void recordCommandBuffer(VkCommandBuffer commandBuffer,
                             uint32_t imageIndex,
                             const Mesh& mesh,
                             const Raster::Pipeline& pipe,
                             VkDescriptorSet *descriptorSet) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        beginRenderPass(commandBuffer, imageIndex);
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe.graphicsPipeline);
            Gra::recordSwapChain(commandBuffer);

            VkBuffer vertexBuffers[] = {mesh.vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(commandBuffer, mesh.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(commandBuffer,
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    pipe.pipelineLayout, // inneholder descriptor layout
                                    0,
                                    1,
                                    descriptorSet, // &m_descriptorSets[Drawing::currSwapFrame], // inneholder uniformbuffer ref
                                    0,
                                    nullptr);

            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.indices.size()), 1, 0, 0, 0);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.indices.size()) - 2, 1, 2, 0, 0);
        }
        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }


    CmdBuffer::CmdBuffer() {
        commandPool = createCommandPool2();
        commandBuffers = createCommandBuffers2();
    }
} // Gra