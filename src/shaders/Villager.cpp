#include "Villager.h"
#include "rendering/Model.h"

namespace Villager {

    Model m_maleVillModel{};
    std::vector<std::shared_ptr<Vill>> m_vills{};

    void initVillModel() {
        m_maleVillModel.init(
                {
                .shaderName = triangle,
                .textureName = "unit.png",
                }
            );
        m_maleVillModel.recreateUboBuffer();
        m_renderModels.emplace_back(&m_maleVillModel); // TODO this whole init is very strange..? No?
    }

    void spawn(float x, float y) {
        auto male = m_vills.emplace_back(std::make_shared<Vill>(Vill{}));
        m_maleVillModel.addEntity(male->entity, true);
        male->entity->size.x = m_maleVillModel.width();
        male->entity->size.y = m_maleVillModel.height();
        male->entity->pos.x = x * male->entity->size.x;
        male->entity->pos.y = y * male->entity->size.y;
    }

    void destroy() {
        m_maleVillModel.destroy();
    }

    std::vector<std::unique_ptr<Vill>> villsWithinBounds(float x0, float y0, float x1, float y1) {
        std::vector<std::unique_ptr<Vill>> foundVills{};

        for (auto &vill : m_vills) {
            if (vill->entity->isWithin(x0, y0, x1, y1)) {
                foundVills.push_back(
                        std::make_unique<Vill>(
                                *vill
                                )
                                );
            }
        }

        return foundVills;
    }

}
