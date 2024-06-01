#include "Map.h"
#include "rendering/Model.h"
#include "shaders/Shaders.h"

void Map::create(int xy) {

    for (int x = 0; x < xy; x++) {
        for (int y = 0; y < xy; y++) {
            auto entity = std::make_shared<Entity>(Entity{
                    .pos = {static_cast<float>(x) * Shaders::m_grassModel.width(),
                            static_cast<float>(y) * Shaders::m_grassModel.height()},
                    .size = {Shaders::m_grassModel.width(),
                             Shaders::m_grassModel.height()},
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
