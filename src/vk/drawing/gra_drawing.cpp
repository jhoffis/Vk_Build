//
// Created by jh on 8/31/2022.
//

#include "gra_drawing.h"
#include "gra_command_buffers.h"
#include "rendering/Model.h"
#include "vk/gra_setup.h"
#include "vk/presentation/gra_swap_chain.h"
#include "vk/setup/gra_logical_device.h"
#include "vk/shading/gra_uniform.h"
#include <stdexcept>

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
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

  VkFenceCreateInfo fenceInfo{
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
      .flags = VK_FENCE_CREATE_SIGNALED_BIT,
  };

  for (size_t i = 0; i < Gra::MAX_FRAMES_IN_FLIGHT; i++) {
    if (vkCreateSemaphore(Gra::m_device, &semaphoreInfo, nullptr,
                          &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(Gra::m_device, &semaphoreInfo, nullptr,
                          &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(Gra::m_device, &fenceInfo, nullptr,
                      &m_inFlightFences[i]) != VK_SUCCESS) {

      throw std::runtime_error(
          "failed to create synchronization objects for a frame!");
    }
  }
}

void drawFrame() {
    vkWaitForFences(Gra::m_device, 1, &m_inFlightFences[currSwapFrame], VK_TRUE,
            UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
            Gra::m_device, Gra::m_swapChain, UINT64_MAX,
            m_imageAvailableSemaphores[currSwapFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        Gra::recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    // Only reset the fence if we are submitting work. To not deadlock on the
    // fence
    vkResetFences(Gra::m_device, 1, &m_inFlightFences[currSwapFrame]);

    float i = 0.0f;

    //        for (auto commandBuffer : Gra::m_commandBuffers[currSwapFrame]) {
    //            vkResetCommandBuffer(commandBuffer, 0);
    //            Gra::recordCommandBuffer(commandBuffer, imageIndex, m_mesh,
    //            Raster::m_pipeline,
    //            &Gra::m_descriptorSets[Drawing::currSwapFrame]); i++;
    //        }
    //        Gra::updateUniformBuffer(currSwapFrame, 1.0f, 1);

    clear = true;
    std::vector<VkCommandBuffer> cmds{};
    for (auto model : m_renderModels) {
        if (!model->visible)
            continue;
        auto cmd = model->renderMeshes(imageIndex);
        if (cmd == nullptr)
            continue;
        cmds.emplace_back(cmd);
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


    // validation layer: Validation Error: [ VUID-vkQueueSubmit-fence-00063 ] Object 0: handle = 0x55b68b4e33c0, type = VK_OBJECT_TYPE_QUEUE; Object 1: handle = 0x2cfba2000000001c, type = VK_OBJECT_TYPE_FENCE; | MessageID = 0x6f35a2b3 | vkQueueSubmit():  (VkFence 0x2cfba2000000001c[]) submitted in SIGNALED state. Fences must be reset before being submitted. The Vulkan spec states: If fence is not VK_NULL_HANDLE, fence must be unsignaled (https://vulkan.lunarg.com/doc/view/1.3.283.0/linux/1.3-extensions/vkspec.html#VUID-vkQueueSubmit-fence-00063)
    // validation layer: Validation Error: [ VUID-vkQueueSubmit-pWaitSemaphores-03238 ] Object 0: handle = 0x95a125000000001a, type = VK_OBJECT_TYPE_SEMAPHORE; Object 1: handle = 0x55b68b4e33c0, type = VK_OBJECT_TYPE_QUEUE; | MessageID = 0xb50452b0 | vkQueueSubmit(): pSubmits[0].pWaitSemaphores[0] queue (VkQueue 0x55b68b4e33c0[]) is waiting on semaphore (VkSemaphore 0x95a125000000001a[]) that has no way to be signaled. The Vulkan spec states: All elements of the pWaitSemaphores member of all elements of pSubmits created with a VkSemaphoreType of VK_SEMAPHORE_TYPE_BINARY must reference a semaphore signal operation that has been submitted for execution and any semaphore signal operations on which it depends must have also been submitted for execution (https://vulkan.lunarg.com/doc/view/1.3.283.0/linux/1.3-extensions/vkspec.html#VUID-vkQueueSubmit-pWaitSemaphores-03238)
    if (vkQueueSubmit(Gra::m_graphicsQueue, 1, &submitInfo,
                m_inFlightFences[currSwapFrame]) != VK_SUCCESS) {
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
    presentInfo.pResults =
        nullptr; // Optional - allows you to specify an array of VkResult values
                 // to check for every individual swap chain if presentation was
                 // successful

    result = vkQueuePresentKHR(Gra::m_presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            Window::m_framebufferResized) {
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
} // namespace Drawing
