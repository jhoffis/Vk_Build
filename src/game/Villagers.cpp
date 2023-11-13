#include "Villagers.h"
#include "gra/Model.h"
#include <cstdlib>

namespace Villager {

    Model m_maleVillModel{};

    void initVillModel() {
        m_maleVillModel.init("triangle", "isometric tileset/separated images/tile_000.png");
        m_maleVillModel.updateUboBuffer();
        m_renderModels.emplace_back(&m_maleVillModel);
    }

    Vill* spawnMale(float x, float y) {
        auto male = new Vill();
        m_maleVillModel.addEntity(&male->entity, true);
        male->entity.size.x = m_maleVillModel.width();
        male->entity.size.y = m_maleVillModel.height();
        male->entity.pos.x = x;
        male->entity.pos.y = y;
        return male;
    }

    void destroy() {
        m_maleVillModel.destroy();
    }

//    void successfulImpregnation(Female &maid, Male &male) {
//        int random_variable;
//    }

}
