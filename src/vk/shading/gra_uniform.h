#pragma once

#include <cstdint>
#include "src/window.h"
#include "math/mat.h"
#include <vector>

namespace Gra {
    extern VkDescriptorSetLayout m_descriptorSetLayout;
    extern std::vector<VkDescriptorSet> m_descriptorSets;

    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage, Math::Mat view, Math::Mat projection);
    void createDescriptorSetLayout();

    void createDescriptorPool();
    void createDescriptorSets();
    void cleanupUniform();
}