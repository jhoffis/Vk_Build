#pragma once

#include "vk/shading/gra_uniform.h"

namespace ShaderSetup {

    std::string getShaderName(const ShaderName &shader);

    Gra_Uniform::UBOMem createMem(const ShaderName &shader);

    void updateRenderUbo(const ShaderName &shaderName,
                         std::shared_ptr<Entity> &entity,
                         Gra_Uniform::UBOMem *uboMem);

    std::vector<VkDescriptorSetLayoutBinding> createBindings(const ShaderName &shader);

    void shaderFillDescriptorSets(const ShaderName &shader,
                                  const Gra_Uniform::UBOMem &uboMem,
                                  const VkImageView &textureImageView,
                                  int size,
                                  const std::vector<VkDescriptorSet> &descriptorSets);
}
