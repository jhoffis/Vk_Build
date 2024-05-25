#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "vk/pipeline/gra_pipeline.h"
#include "vk/drawing/gra_command_buffers.h"
#include "vk/shading/gra_uniform.h"
#include "Entity.h"
#include "Mesh2D.h"
#include "src/shaders/consts/ShaderName.h"

struct UniformStuff {
    // what is common and can be used to generate the rest?
    int bindingId;
    bool vertexOrFragment;
    int type; // like is it image or ubo or smt else?
};


struct ModelInfo {
    const float fallbackWidth{0.f}, fallbackHeight{0.f};
    const ShaderName &shaderName;
    const char *textureName{};
};

/*
 * super inefficient first model version
 * Think of this class as a collection of a type of model instead of a single object to render.
 *
 * TODO for senere, hva om du definerer alle objekt du ønsker og så init de senere?
*/
struct Model {

    ShaderName shaderName;

    Mesh2D mesh{};
    Gra::CmdBuffer cmdBuffer{};
    Gra_Uniform::UBOMem uboMem{};

    Raster::Pipeline pipeline{};
    VkImageView texImageView;
    VkDescriptorPool pool;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets{};

    std::vector<std::shared_ptr<Entity>> entities{};
    bool visible = true;

    void init(ModelInfo info);

    void recreateUboBuffer();
    void addEntity(std::shared_ptr<Entity> entity, bool update);

    VkCommandBuffer renderMeshes(uint32_t imageIndex);

    float width() const {
        return mesh.worldWidth;
    }

    float height() const {
        return mesh.worldHeight;
    }
    void createPipeline();

    void destroy();

};
#ifdef RMDEV
void recreateModelPipelines();
#endif

void destroyModels();

extern std::vector<Model *> m_renderModels;


