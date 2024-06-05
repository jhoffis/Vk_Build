#include "Villager.h"
#include "rendering/Model.h"
#include "shaders/Shaders.h"
#include "Map.h"
#include "timer_util.h"

namespace Villager {

    std::vector<Vill> m_vills{};
    std::vector<Vill*> m_selectedVills{}; //TODO should it be selected entities instead? Hm...

    void initVillModel() {
        Shaders::m_villModel.recreateUboBuffer();
    }

    float z = 0;

    void spawn(float x, float y) {
        z = 0.1f;
        auto male = m_vills.emplace_back(Vill{
                .entity = std::make_shared<Entity>(Entity{
                        .pos = {x * Map::tileSize, y * Map::tileSize, z},
                        .size = {Shaders::m_villModel.width(), Shaders::m_villModel.height()},
                }),
        });
        Shaders::m_villModel.addEntity(male.entity, true);
    }

    void destroy() {
        Shaders::m_villModel.destroy();
    }

    // TODO denne kopierer vill
    void villsWithinBounds(float x0,
                                                         float y0,
                                                         float x1,
                                                         float y1,
                                                         bool store) {
        std::vector<Vill*> foundVills{};
        auto sizeOfBox = std::abs((x0 - x1) + (y0 - y1));

        if (sizeOfBox < 0.01) { // select one
            for (unsigned long i = m_vills.size() - 1; i < m_vills.size(); i--) {
                if (m_vills[i].entity->isWithin(x0, y0, x1, y1)) {
                    foundVills.emplace_back(&m_vills[i]);
                    break;
                }
            }
        } else {
            for (unsigned long i = m_vills.size() - 1; i < m_vills.size(); i--) {
                if (m_vills[i].entity->isWithin(x0, y0, x1, y1)) {
                    foundVills.emplace_back(&m_vills[i]);
                }
            }
        }
        if (store) {
            m_selectedVills.clear();
            for (auto &vill: foundVills) {
                vill->entity->selected = true;
                m_selectedVills.emplace_back(vill);
            }
        }

    }

    void sort() {
//    TODO    std::sort(m_vills.begin(), m_vills.end(),
//                  [](auto a, auto b) {
//                      return a.entity->pos.y > b.entity->pos.y;
//                  });
    }

    void update() {
        for (auto &vill: m_vills) {
            vill.update(Timer::delta());
        }
    }

    void unselectAll() {
        for (auto vill: m_vills) {
            vill.entity->selected = false;
        }
    }

    void Vill::update(double delta) {
        if (!path.empty()) {
            const auto movementSpeed = .5f * delta;

            auto originalApproach = path[pathIndex] - entity->pos;
            auto approach = originalApproach;
            approach.normalize();
            approach *= movementSpeed;
            if (approach > originalApproach || (originalApproach.x == 0 && originalApproach.y == 0)) {
                approach = originalApproach;
                pathIndex++;
                if (pathIndex == path.size()) {
                    path.clear();
                }
            }

            entity->pos.x += approach.x;
            entity->pos.y += approach.y;
        }

//        auto angle = 90;
//
//        auto dx = std::cos(angle) * movementSpeed;
//        auto dy = std::sin(angle) * movementSpeed;
//
//        entity->pos.x += dx;
//        entity->pos.y += dy;
    }
}
