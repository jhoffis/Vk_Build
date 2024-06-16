#pragma once

#include <cstdint>
#include <memory>
#include "rendering/Entity.h"
#include "game/vills/Femaleness.h"
#include "game/vills/Maleness.h"

namespace Villager {

    void initVillModel();
    void spawn(float x, float y, bool male);

    void villsWithinBounds(float x0,
                                                         float y0,
                                                         float x1,
                                                         float y1,
                                                         bool store);
    void sort(std::vector<std::shared_ptr<Vill>> vills);
    void sort();
    void update();
    void destroy();
}
