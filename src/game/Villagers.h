#pragma once

#include <cstdint>
#include "gra/Entity.h"
#include "Femaleness.h"
#include "Maleness.h"

namespace Villager {

    void successfulImpregnation(Female& maid, Male& male);

    void initVillModel();
    Male* spawnMale(float x, float y);
    void destroy();
}