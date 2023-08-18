#pragma once

#include "gra/Entity.h"

namespace Villager {

    struct Vill {
        Entity *entity;
    };

    void createVillagers();
    void destroy();
}