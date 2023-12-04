#include "Villagers.h"
#include "gra_elems/Model.h"
#include "vk/shading/gra_vertex.h"
#include <cstdlib>

namespace Villager {

    Model m_maleVillModel{};

    void initVillModel() {
        m_maleVillModel.init(
                {
                .sizeOfUBO = sizeof(Gra::UniformBufferObject),
                .shaderName = "triangle",
                .textureName = "unit.png",
                .bindings = {
                    {
                        .binding = 0,
                        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                        .descriptorCount = 1,
                        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                        .pImmutableSamplers = nullptr // only relevant for image sampling related descriptor,
                    },
                    {
                        .binding = 1,
                        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                        .descriptorCount = 1,
                        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, // fragment shader.  It is possible to use texture sampling in the vertex shader, for example to dynamically deform a grid of vertices by a heightmap
                        .pImmutableSamplers = nullptr,
                    }
                }
                }
                );
        m_maleVillModel.updateUboBuffer();
        m_renderModels.emplace_back(&m_maleVillModel);
    }

    Vill* spawnMale(float x, float y) {
        auto male = new Vill();
        m_maleVillModel.addEntity(&male->entity, true);
        male->entity.size.x = m_maleVillModel.width();
        male->entity.size.y = m_maleVillModel.height();
        male->entity.pos.x = x * male->entity.size.x;
        male->entity.pos.y = y * male->entity.size.y;
        return male;
    }

    void destroy() {
        m_maleVillModel.destroy();
    }


}
