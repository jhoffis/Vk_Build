#include "Map.h"
#include "gra_elems/Model.h"

Model grassModel{};

std::vector<Entity*> grass;

void Map::create(int xy) {
    grassModel.init("triangle", "grass.png");

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
