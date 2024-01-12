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
    }

    void spawn(float x, float y) {
        auto male = m_vills.emplace_back(std::make_shared<Vill>(Vill{
            .entity = std::make_shared<Entity>(Entity{
                    .pos = {x * m_maleVillModel.width(), y * m_maleVillModel.height()},
                    .size = {m_maleVillModel.width(), m_maleVillModel.height()},
            }),
        }));
        m_maleVillModel.addEntity(male->entity, true);
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
