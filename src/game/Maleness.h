#pragma once

#include "Vill.h"

/*
 * Actions only males can do
 */

namespace Villager {

    struct Penis {
        int8_t beauty;
        int8_t length;
        int8_t spermQuality; // increases chance for reproduction and bettering/stability of genes
        int8_t pleasureHavingQuality;
    };

    struct Male {
        Villager::Vill vill{};
        Penis reproductiveOrgan{};
    };


}