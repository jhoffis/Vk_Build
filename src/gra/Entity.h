//
// Created by jonhof on 8/17/2023.
//

#ifndef VULKAN_ENTITY_H
#define VULKAN_ENTITY_H


#include "math/Vec3.h"

struct Entity {
    Vec3 pos{};
    bool visible{};
};


#endif //VULKAN_ENTITY_H
