//
// Created by jonhof on 8/17/2023.
//

#include "Entity.h"

// TODO snu om på pos og worldPos for ellers vil jeg slite med å håndtere flytting og hastighet av forskjellige bilder og units.
float convertToWorldCoordinates(float pos, float size) {
    return pos * size;
}

bool Entity::isAbove(float x, float y) const {
    auto posX = convertToWorldCoordinates(pos.x, size.x);
    auto posY = convertToWorldCoordinates(pos.y, size.y);
    return x >= posX && y >= posY
        && x < posX + size.x && y < posY + size.y;
}
