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
    VkImageView texImageView;
    VkDescriptorPool pool;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets{};


public:
    std::vector<Entity> entities{};
    bool visible = true;

    void init(const std::string &shaderName, const std::string &textureName);

    void updateUboBuffer();
    Entity* addEntity(bool update = true);

    VkCommandBuffer renderMeshes(uint32_t imageIndex);

    float width() const {
        return mesh.worldWidth;
    }

    float height() const {
        return mesh.worldHeight;
    }

    void destroy();

};

extern std::vector<Model *> m_renderModels;
