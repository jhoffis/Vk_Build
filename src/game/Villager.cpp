#include "Villager.h"
#include "gra/Model.h"

Model villModel{};

void Villager::createVillagers() {
    villModel.init("triangle", "unit.png");
    for (int x = 0; x < 3; x++) {
        auto entity = villModel.addEntity(false);
        entity->pos.x = villModel.width() * static_cast<float>(x);
        entity->pos.y = villModel.height() * static_cast<float>(1);
    }
    villModel.updateUboBuffer();
    m_renderModels.emplace_back(&villModel);
}

void Villager::destroy() {
    villModel.destroy();
}
