#include "Villager.h"
#include "rendering/Model.h"
#include "shaders/Shaders.h"

namespace Villager {

    std::vector<std::shared_ptr<Vill>> m_vills{};

    void initVillModel() {
        Shaders::m_villModel.recreateUboBuffer();
    }

    float z = 0;

    void spawn(float x, float y) {
        z = 0.1f;
        auto male = m_vills.emplace_back(std::make_shared<Vill>(Vill{
            .entity = std::make_shared<Entity>(Entity{
                    .pos = {x * Shaders::m_villModel.width(), y * Shaders::m_villModel.height(), z},
                    .size = {Shaders::m_villModel.width(), Shaders::m_villModel.height()},
            }),
        }));
        Shaders::m_villModel.addEntity(male->entity, true);
    }

    void destroy() {
        Shaders::m_villModel.destroy();
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

    void unselectAll() {
        for (auto vill : m_vills) {
            vill->entity->selected = false;
        }
    }

}
