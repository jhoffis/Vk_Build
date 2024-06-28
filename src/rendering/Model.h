#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "vk/gra_descriptors.h"
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
/*
 * super inefficient first model version
 * Think of this class as a collection of a type of model instead of a single object to render.
 *
*/
struct Model {

    bool visible = true;
    bool queueRecreateUboBuffer = false;

    const std::string shaderName{};
    std::vector<std::shared_ptr<Entity>> entities{}; // TODO make not contain pointers
    Gra::CmdBuffer cmdBuffer{};
    Gra_desc::DescriptorBox box{};
    Raster::Pipeline pipeline{};
    Mesh2D mesh{};

    std::function<void(Gra_Uniform::UBOMem*, 
                       const std::shared_ptr<Entity> &,
                       const uint16_t index)> updateRenderUbo{};
    std::function<void(const Model &model)> renderPasses{};
    std::function<void(Gra_Uniform::UBOMem*)> initRenderUbo{};

    Model(const std::string &shaderName);

    void init(const uint16_t countInstances);
    void createPipeline();
    VkCommandBuffer renderMeshes(uint32_t imageIndex);
    void sort();
    std::shared_ptr<Entity> spawn(Vec2 mapPos, std::string texture);
    void removeEntity(const std::shared_ptr<Entity>&  sharedPtr);
    void destroy();
};


#ifdef RMDEV
void recreateModelPipelines();
#endif

void destroyModels();

extern std::vector<Model *> m_renderModels;

namespace Shaders {
//     extern Model m_grassModel;
//     extern Model m_houseModel;
    extern Model m_villModel;
    extern Model m_selectionBoxModel;
}
