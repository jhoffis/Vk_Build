#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "vk/pipeline/gra_pipeline.h"
#include "vk/drawing/gra_command_buffers.h"
#include "vk/shading/gra_uniform.h"
#include "Entity.h"
#include "Mesh2D.h"

struct UniformStuff {
    // what is common and can be used to generate the rest?
    int bindingId;
    boolean vertexOrFragment;
    int type; // like is it image or ubo or smt else?
};


struct ModelInfo {
    int sizeOfUBO{};
    float fallbackWidth{0.f}, fallbackHeight{0.f};
    char const *shaderName{};
    char const *textureName{};
    const std::vector<VkDescriptorSetLayoutBinding> bindings{};
};

/*
 * super inefficient first model version
 * Think of this class as a collection of a type of model instead of a single object to render.
 *
 * TODO for senere, hva om du definerer alle objekt du ønsker og så init de senere?
*/
struct Model {
private:
    Mesh2D mesh{};
    Gra::CmdBuffer cmdBuffer{};
    Gra::UBOMem uboMem{};

    Raster::Pipeline pipeline{};
    VkImageView texImageView;
    VkDescriptorPool pool;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets{};


public:



    std::vector<Entity*> entities{};
    bool visible = true;

    void init(ModelInfo modelInfo);

    void updateUboBuffer();
    void addEntity(Entity* entity, bool update);

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


