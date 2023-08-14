//
// Created by jonah on 8/5/2023.
//

#ifndef VULKAN_MODEL_H
#define VULKAN_MODEL_H


#include <vulkan/vulkan.h>
#include <vector>
#include "vk/pipeline/gra_pipeline.h"
#include "vk/drawing/gra_command_buffers.h"
#include "vk/shading/gra_uniform.h"

/*
 * super inefficient first model version
 * Think of this class as a collection of a type of model instead of a single object to render.
 *
 * TODO for senere, hva om du definerer alle objekt du ønsker og så init de senere?
*/
class Model {
private:
    VkDescriptorPool pool;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets{};
    Raster::Pipeline pipeline{};
    Gra::CmdBuffer cmdBuffer{};
    Mesh mesh{};
    Gra::StandardUBOMem uboMem{};
public:
    bool visible = true;
    float x = 0;

    Model();

    void destroy();

    VkCommandBuffer renderMeshes(uint32_t imageIndex);
};

extern std::vector<Model> m_renderModels;

#endif //VULKAN_MODEL_H
