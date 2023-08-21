#pragma once

#include <cstdint>
#include "gra/Entity.h"
#include "Femaleness.h"
#include "Maleness.h"

namespace Villager {

    void successfulImpregnation(Female& mother, Male& father);

    void createVillagers();
    void destroy();
}