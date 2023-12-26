#include "Map.h"
#include "gra_elems/Model.h"
#include "vk/shading/gra_vertex.h"

Model grassModel{};

std::vector<RenderEntity *> grass;

void Map::create(int xy) {
    grassModel.init(
            {
                    .shaderName = triangle,
                    .textureName = "grass.png",
            }
    );

    for (int x = 0; x < xy; x++) {
        for (int y = 0; y < xy; y++) {
            auto entity = new RenderEntity();
            entity->size.x = grassModel.width();
            entity->size.y = grassModel.height();
            entity->pos.x = static_cast<float>(x) * entity->size.x;
            entity->pos.y = static_cast<float>(y) * entity->size.y;
            entity->visible = true;
            auto p_entity = grass.emplace_back(entity);
            grassModel.addEntity(p_entity, false);
        }
    }
    grassModel.recreateUboBuffer();
    m_renderModels.emplace_back(&grassModel);
}

void Map::destroy() {
    grassModel.destroy();
}
