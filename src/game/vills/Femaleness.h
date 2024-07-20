#pragma once

#include "models/Villager.h"
#include <vector>
/*
 * Actions only females can do
 */

namespace Villager {

    struct Female {
        Vill vill{};
        std::vector<Vill> foeti{};
    };

}
