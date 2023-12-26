#pragma once

#include <cstdint>
#include <memory>
#include "rendering/Entity.h"
#include "game/vills/Femaleness.h"
#include "game/vills/Maleness.h"

namespace Villager {

    void initVillModel();
    void spawn(float x, float y);

    std::vector<std::unique_ptr<Vill>> villsWithinBounds(float x0, float y0, float x1, float y1);

    void destroy();
}