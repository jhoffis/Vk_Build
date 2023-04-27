//
// Created by jonhof on 4/26/2023.
//

#include "shader.h"

namespace Shader {

    std::vector<ShaderData> m_shaders{};

    ShaderData create(const Texture::TexData &tex) {
        ShaderData shader{};
        Gra::createDescriptorSetLayout(shader.ubo);
        Gra::createUniformBuffers(shader.ubo);
        Gra::createDescriptorPool(shader.ubo);
        Gra::createDescriptorSets(tex, shader.ubo);
        shader.pipeline = Gra::createGraphicsPipeline(shader.ubo);
        m_shaders.push_back(shader);
        return shader;
    }

    void cleanup() {
        for (const auto& shader : m_shaders) {
            Gra::cleanupUniform(shader.ubo);
            vkDestroyPipeline(Gra::m_device, shader.pipeline.graphicsPipeline, nullptr);
            vkDestroyPipelineLayout(Gra::m_device, shader.pipeline.pipelineLayout, nullptr);
        }
    }
}
