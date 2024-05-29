#pragma once

#include "vk/shading/gra_uniform.h"

namespace ShaderSetup {

    Gra_Uniform::UBOMem createMem(const ShaderName &shader);

    void updateRenderUbo(const ShaderName &shaderName,
                         const std::shared_ptr<Entity> &entity,
                         Gra_Uniform::UBOMem &uboMem);

    std::vector<VkDescriptorSetLayoutBinding> createBindings(const ShaderName &shader);

    std::vector<VkDescriptorSet> createDescriptorSets(const ShaderName &shader,
                                                      VkDescriptorSetLayout &layout,
                                                      VkDescriptorPool &pool,
                                                      Gra_Uniform::UBOMem &uboMem,
                                                      VkImageView &textureImageView);
}
