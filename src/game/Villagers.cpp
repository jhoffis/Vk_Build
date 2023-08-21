#include "Villagers.h"
#include "gra/Model.h"

namespace Villager {

    Model villModel{};

    void createVillagers() {
        villModel.init("triangle", "unit.png");
        for (int x = 0; x < 3; x++) {
            auto entity = villModel.addEntity(false);
            entity->pos.x = villModel.width() * static_cast<float>(x);
            entity->pos.y = villModel.height() * static_cast<float>(1);
        }
        villModel.updateUboBuffer();
        m_renderModels.emplace_back(&villModel);
    }

    void destroy() {
        villModel.destroy();
    }

    void successfulImpregnation(Female &mother, Male &father) {
        mother.pregnant = true;
    }

}
