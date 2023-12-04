#include "Map.h"
#include "gra_elems/Model.h"
#include "vk/shading/gra_vertex.h"

Model grassModel{};

std::vector<Entity *> grass;

void Map::create(int xy) {
    grassModel.init(
        {
            .sizeOfUBO = sizeof(Gra::UniformBufferObject),
            .shaderName = "triangle",
            .textureName = "grass.png",
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

    for (int x = 0; x < xy; x++) {
        for (int y = 0; y < xy; y++) {
            auto entity = new Entity();
            entity->size.x = grassModel.width();
            entity->size.y = grassModel.height();
            entity->pos.x = static_cast<float>(x) * entity->size.x;
            entity->pos.y = static_cast<float>(y) * entity->size.y;
            entity->visible = true;
            auto p_entity = grass.emplace_back(entity);
            grassModel.addEntity(p_entity, false);
        }
    }
    grassModel.updateUboBuffer();
    m_renderModels.emplace_back(&grassModel);
}

void Map::destroy() {
    grassModel.destroy();
}
