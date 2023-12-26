#pragma once

#include <cstdint>
#include <memory>
#include "gra_elems/RenderEntity.h"
#include "src/game/vills/Femaleness.h"
#include "src/game/vills/Maleness.h"

namespace Villager {

    void initVillModel();
    void spawn(float x, float y);

    std::vector<std::unique_ptr<Vill>> villsWithinBounds(float x0, float y0, float x1, float y1);

    void destroy();
}