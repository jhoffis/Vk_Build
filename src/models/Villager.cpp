#include "Villager.h"
#include "rendering/Model.h"
#include "shaders/Shaders.h"
#include "Map.h"
#include "timer_util.h"
#include "path_finding.h"
#include <iostream>

namespace Villager {

    std::vector<Vill> m_vills{};
    std::vector<Vill*> m_selectedVills{}; //TODO should it be selected entities instead? Hm...
    float z = 0;

    void init() {
        Shaders::m_villModel.box = Gra_desc::createDescriptorBox(1, {
                    Gra_desc::DescriptorBindInfo {
                    .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    .bindingNum = 0,
                    .count = 100,
                    .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                    .sizeofUBO = sizeof(Gra::UniformBufferObject), 
                    },
                    Gra_desc::DescriptorBindInfo {
                    .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    .bindingNum = 1,
                    .count = 2,
                    .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                    .textureNames = {"male.png", "female.png"}
                    },
                    });
        Shaders::m_villModel.init(100);
    }

    void spawn(float x, float y, bool male) {
        z = 0.1f;
        auto vill = m_vills.emplace_back(Vill{
                .entity = Shaders::m_villModel.spawn({x, y},
                        {male ? "male.png" : "female.png"})
                });
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
        std::cout << "Mouse bounds: x0="<<x0<<", y0="<<y0<<", x1="<<x1<<", y1="<<y1<<std::endl;
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
                std::cout << "Entity pos: x="<<m_vills[i].entity->pos.x<<", y="<<m_vills[i].entity->pos.y<<", scale: x="<<m_vills[i].entity->scale.x<<", y="<<m_vills[i].entity->scale.y<<std::endl;
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

    /*
     * TODO
     * Maybe for each next point, check the latest/last vec2 and see if
     * a line straight from the current point would hit anything? Maybe.
     *
     * Maybe if I want to stick to specific animations and not allow for
     * floating movement like scouts in aok and aoc, then I can from the
     * approach variable pick the closest eq predefined normalized vec2.
     */
    void Vill::update(double delta) {
        if (!path.empty()) {
            const auto movementSpeed = .5f * delta;

//            auto index = PathFinder::nextClearLineTilesDDA(
//                    *Map::m_map,
//                    Vec2{entity->pos.x, entity->pos.y},
//                    path,
//                    pathIndex
//            );

            auto originalApproach = path[pathIndex] - entity->pos;
            auto approach = originalApproach;
            approach.normalize();
            approach *= movementSpeed;
            // TODO handle if it lags and it will predict when it should actually hit the right frame.
            if (approach > originalApproach || (Vec2{0.001, 0.001} > originalApproach)) {
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
