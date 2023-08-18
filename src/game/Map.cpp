#include "Map.h"
#include "src/gra/Model.h"

Model grassModel{};

void Map::create(int xy) {
    grassModel.init("triangle", "grass.png");

    for (int x = 0; x < xy; x++) {
        for (int y = 0; y < xy; y++) {
            auto entity = grassModel.addEntity(false);
            entity->pos.x = grassModel.width() * static_cast<float>(x);
            entity->pos.y = grassModel.height() * static_cast<float>(y);
        }
    }
    grassModel.updateUboBuffer();
    m_renderModels.emplace_back(&grassModel);
}

void Map::destroy() {
    grassModel.destroy();
}
