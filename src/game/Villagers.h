#pragma once

#include <cstdint>
#include "gra_elems/Entity.h"
#include "Femaleness.h"
#include "Maleness.h"

namespace Villager {

//    void successfulImpregnation(Female& maid, Male& male);

    void initVillModel();
    Vill* spawnMale(float x, float y);
    void destroy();
}