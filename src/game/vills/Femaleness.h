#pragma once

#include "Vill.h"
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