#pragma once

#include "Vill.h"

/*
 * Actions only females can do
 */

namespace Villager {

    struct Female {
        Villager::Vill vill{};
        bool pregnant{};
    };

}