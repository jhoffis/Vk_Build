//
// Created by jonhof on 4/26/2023.
//

#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H


#include "gra_uniform.h"
#include "vk/pipeline/gra_pipeline.h"
#include "vk/gra_setup.h"
#include <vector>

namespace Shader {

    struct ShaderData {

        Gra::Uniform ubo{};
        Gra::Pipeline pipeline{};

        // Det er vel kanskje greit å deklarere en ny struct for hver shader? Kanskje... hm. nja hva om jeg ønsker å ha shader editing hotreloading?

        void bindPipeline(VkCommandBuffer commandBuffer) const {
            vkCmdBindPipeline(commandBuffer,
                              VK_PIPELINE_BIND_POINT_GRAPHICS,
                              pipeline.graphicsPipeline
            );
        }
    };

    ShaderData create(const Texture::TexData &tex);
    void cleanup();
}
#endif //VULKAN_SHADER_H
