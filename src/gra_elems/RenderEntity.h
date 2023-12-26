#pragma once

#include "math/Vec3.h"
#include "math/Vec2.h"

struct RenderEntity {
    Vec3 pos{};
    Vec2 size{};
    bool visible{};

    constexpr bool isAbove(float x, float y) const;

    constexpr bool isWithin(float xTL, float yTL, float xBR, float yBR) const {
        return xTL <= pos.x + .5*size.x && yTL <= pos.y + .5*size.y
               && xBR >= pos.x + .5*size.x && yBR >= pos.y + .5*size.y;
    }
};
