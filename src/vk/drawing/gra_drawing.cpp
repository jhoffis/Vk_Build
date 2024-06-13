//
// Created by jh on 8/31/2022.
//

#include <stdexcept>
#include "gra_drawing.h"
#include "vk/gra_setup.h"
#include "vk/presentation/gra_swap_chain.h"
#include "gra_command_buffers.h"
#include "vk/setup/gra_logical_device.h"
#include "vk/shading/gra_uniform.h"
#include "rendering/Model.h"

namespace Drawing {

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    uint32_t currSwapFrame = 0;

    bool clear;

    void createSyncObjects() {
        m_imageAvailableSemaphores.resize(Gra::MAX_FRAMES_IN_FLIGHT);
        m_renderFinishedSemaphores.resize(Gra::MAX_FRAMES_IN_FLIGHT);
        m_inFlightFences.resize(Gra::MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };

        VkFenceCreateInfo fenceInfo{
                .sType =     VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                .flags = VK_FENCE_CREATE_SIGNALED_BIT,
        };

        for (size_t i = 0; i < Gra::MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(Gra::m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(Gra::m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(Gra::m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {

                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }

    void drawFrame() {
        vkWaitForFences(Gra::m_device, 1, &m_inFlightFences[currSwapFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(Gra::m_device, Gra::m_swapChain, UINT64_MAX, m_imageAvailableSemaphores[currSwapFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            Gra::recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        // Only reset the fence if we are submitting work. To not deadlock on the fence
        vkResetFences(Gra::m_device, 1, &m_inFlightFences[currSwapFrame]);

        float i = 0.0f;

//        for (auto commandBuffer : Gra::m_commandBuffers[currSwapFrame]) {
//            vkResetCommandBuffer(commandBuffer, 0);
//            Gra::recordCommandBuffer(commandBuffer, imageIndex, m_mesh, Raster::m_pipeline, &Gra::m_descriptorSets[Drawing::currSwapFrame]);
//            i++;
//        }
//        Gra::updateUniformBuffer(currSwapFrame, 1.0f, 1);

        clear = true;
        std::vector<VkCommandBuffer> cmds{};
        for (auto model : m_renderModels) {
            if (!model->visible)
                continue;
            cmds.emplace_back(model->renderMeshes(imageIndex));
            clear = false;
        }
        
        // TODO change this to VkSubmitInfo2 ?
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphores[currSwapFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = cmds.size();
        submitInfo.pCommandBuffers = cmds.data();

        VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphores[currSwapFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(Gra::m_graphicsQueue, 1, &submitInfo, m_inFlightFences[currSwapFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {Gra::m_swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional - allows you to specify an array of VkResult values to check for every individual swap chain if presentation was successful

        result = vkQueuePresentKHR(Gra::m_presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Window::m_framebufferResized) {
            Window::m_framebufferResized = false;
            Gra::recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currSwapFrame = (currSwapFrame + 1) % Gra::MAX_FRAMES_IN_FLIGHT;
    }

    void cleanupSyncObjects() {
        for (size_t i = 0; i < Gra::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(Gra::m_device, m_imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(Gra::m_device, m_renderFinishedSemaphores[i], nullptr);
            vkDestroyFence(Gra::m_device, m_inFlightFences[i], nullptr);
        }
    }
} // Gra