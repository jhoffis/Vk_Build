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

    Gra::CmdBuffer cmdBuffer{};

    /* These are what is put into the cmdbuffer:
       Like maybe this should all be somehow referred to in each entity or smt?
       Because I want one entity to be oversized and one to have a different texture,
       but they still use the same shader and are of the same "type"
       I just want to say like hey, create this dude or thing,
       it is this big atm and he uses this texture, and he is here.
       I don't want to deal with models and so on, unless I have to.
       Like just auto dude = model.spawn(size: {2,2}, pos: {1, 1}, tex: "dude");

       whereas now I have to write like auto male = m_vills.emplace_back(Vill{
                .entity = std::make_shared<Entity>(Entity{
                        .pos = {x * Map::tileSize, y * Map::tileSize, z},
                        .size = {Shaders::m_villModel.width(), Shaders::m_villModel.height()},
                }),
        });
        Shaders::m_villModel.addEntity(male.entity, true);
    */
    Entity spawn(Vec2 mapPos, std::string texture);

    Mesh2D mesh{};
    Gra_Uniform::UBOMem uboMem{};
    std::vector<VkImageView> texImageViews{};
    std::vector<VkDescriptorSet> descriptorSets{};

    Raster::Pipeline pipeline{};
    VkDescriptorPool pool{};
    VkDescriptorSetLayout descriptorSetLayout{};

    std::vector<std::shared_ptr<Entity>> entities{};
    bool visible = true;
    bool queueRecreateUboBuffer = false;

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

    void setTextures(const std::vector<std::string> &textures);

    VkCommandBuffer renderMeshes(uint32_t imageIndex);

    [[nodiscard]] float width() const {
        return mesh.worldWidth;
    }

    [[nodiscard]] float height() const {
        return mesh.worldHeight;
    }
    void createPipeline();

    void destroy();

    void runRecreateUbo();
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