#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "vk/pipeline/gra_pipeline.h"
#include "vk/drawing/gra_command_buffers.h"
#include "vk/shading/gra_uniform.h"
#include "Entity.h"
#include "Mesh2D.h"
#include "models/consts/ShaderName.h"
#include <functional>

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

    const std::string shaderName;
    const std::function<void(Gra_Uniform::UBOMem*, const std::shared_ptr<Entity> &)> updateRenderUbo;
    const std::vector<ShaderComponentOrder> order;
    const int sizeOfUBO;
    const float overrideWidth;
    const float overrideHeight;
    const std::vector<std::string> textures;

    Mesh2D mesh{};
    Gra::CmdBuffer cmdBuffer{};
    Gra_Uniform::UBOMem uboMem{};

    Raster::Pipeline pipeline{};
    std::vector<VkImageView> texImageViews{};
    VkDescriptorPool pool{};
    VkDescriptorSetLayout descriptorSetLayout{};
    std::vector<VkDescriptorSet> descriptorSets{};

    std::vector<std::shared_ptr<Entity>> entities{};
    bool visible = true;

    Model(std::string shaderName,
          std::function<void(Gra_Uniform::UBOMem*, const std::shared_ptr<Entity> &entity)> updateRenderUbo,
          std::vector<ShaderComponentOrder> order,
          int sizeOfUBO,
          float overrideWidth,
          float overrideHeight,
          std::vector<std::string> textures);

    void init();

    [[nodiscard]] std::vector<VkDescriptorSet> createDescriptorSets() const;
    void recreateUboBuffer();
    void addEntity(const std::shared_ptr<Entity>& entity, bool update);
    void removeEntity(const std::shared_ptr<Entity>&  sharedPtr);
    void spawn(float x, float y);

    void sort();

    VkCommandBuffer renderMeshes(uint32_t imageIndex);

    [[nodiscard]] float width() const {
        return mesh.worldWidth;
    }

    [[nodiscard]] float height() const {
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

namespace Shaders {
    extern Model m_grassModel;
    extern Model m_houseModel;
    extern Model m_villModel;
    extern Model m_selectionBoxModel;
}