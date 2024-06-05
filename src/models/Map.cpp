#include "Map.h"
#include "rendering/Model.h"
#include "shaders/Shaders.h"

void Map::createVisual(int xy) {

    for (int x = 0; x < xy; x++) {
        for (int y = 0; y < xy; y++) {
            auto entity = std::make_shared<Entity>(Entity{
                    .pos = {static_cast<float>(x) * tileSize,
                            static_cast<float>(y) * tileSize},
                    .size = {tileSize,
                             tileSize},
                    .visible = true,
            });
            Shaders::m_grassModel.addEntity(entity, false);
        }
    }
    Shaders::m_grassModel.recreateUboBuffer();
}

void Map::destroy() {
    Shaders::m_grassModel.destroy();
}

Vec2 Map::Map::indexToWorld(int i) const {
    return Vec2(
            static_cast<float>(i % xy) * tileSize,
            static_cast<float>(i / xy) * tileSize
    );
}
