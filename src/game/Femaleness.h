#pragma once

#include "Vill.h"
#include <vector>
/*
 * Actions only females can do
 */

namespace Villager {

    struct Breasts {
        int8_t beauty;
        int8_t size;
        int8_t milkQuality;
        int8_t milkQuantity;
        int8_t pleasureHavingQuality;
    };

    struct Vulva {
        int8_t beauty;
        int8_t length, tightness;
        int8_t eggQuality;
        int8_t pleasureHavingQuality;
    };

    struct Female {
        Vill vill{};
        std::vector<Vill> foeti{};
        Breasts breasts{};
        Vulva reproductiveOrgan{};
    };

}