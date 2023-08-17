#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "vk/pipeline/gra_pipeline.h"
#include "vk/drawing/gra_command_buffers.h"
#include "vk/shading/gra_uniform.h"
#include "Entity.h"

/*
 * super inefficient first model version
 * Think of this class as a collection of a type of model instead of a single object to render.
 *
 * TODO for senere, hva om du definerer alle objekt du ønsker og så init de senere?
*/
class Model {
private:
    Mesh2D mesh{};
    Gra::CmdBuffer cmdBuffer{};
    Gra::StandardUBOMem uboMem{};

    Raster::Pipeline pipeline{};

    VkDescriptorPool pool;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets{};

    std::vector<Entity> entities{};

public:
    bool visible = true;

    Model(const std::string &shaderName, const std::string &textureName);

    Entity* addEntity();

    VkCommandBuffer renderMeshes(uint32_t imageIndex);

    void destroy();
};

extern std::vector<Model *> m_renderModels;
