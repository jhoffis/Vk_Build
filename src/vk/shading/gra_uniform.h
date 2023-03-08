#pragma once

#include <cstdint>
#include "src/window.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Gra {
    extern VkDescriptorSetLayout m_descriptorSetLayout;
    extern std::vector<VkDescriptorSet> m_descriptorSets;

    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage, glm::mat4 view, glm::mat4 projection);
    void createDescriptorSetLayout();

    void createDescriptorPool();
    void createDescriptorSets();
    void cleanupUniform();
}