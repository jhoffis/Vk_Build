//
// Created by jh on 8/31/2022.
//
#include "gra_framebuffers.h"
#include "src/vk/presentation/gra_image_views.h"
#include "src/vk/pipeline/gra_render_passes.h"
#include "src/vk/pipeline/gra_multisampling.h"
#include "src/vk/presentation/gra_swap_chain.h"
#include "src/vk/shading/gra_depth.h"
#include <stdexcept>
#include <array>

namespace Gra {

    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    void createFramebuffers() {
        m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

        for (size_t i = 0; i < m_swapChainImageViews.size(); i++) {
            std::array<VkImageView, 3> attachments = {
                colorImageView,
                depthImageView,
                m_swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = m_swapChainExtent.width;
            framebufferInfo.height = m_swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
} // Gra