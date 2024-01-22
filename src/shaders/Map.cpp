#include "Map.h"
#include "rendering/Model.h"

Model grassModel{};

void Map::create(int xy) {
    grassModel.init(
            {
                    .shaderName = grass,
                    .textureName = "grass.png",
            }
    );

    for (int x = 0; x < xy; x++) {
        for (int y = 0; y < xy; y++) {
            auto entity = std::make_shared<Entity>(Entity{
                    .pos = {static_cast<float>(x) * grassModel.width(),
                            static_cast<float>(y) * grassModel.height()},
                    .size = {grassModel.width(),
                             grassModel.height()},
                    .visible = true,
            });
            grassModel.addEntity(entity, false);
        }
    }
    grassModel.recreateUboBuffer();
}

void Map::destroy() {
    grassModel.destroy();
}
