#pragma once

#include "math/Vec3.h"
#include "math/Vec2.h"
#include "vk/shading/gra_uniform.h"

struct Entity {
    Vec3 pos{};
    Vec2 size{};
    bool visible{};
    bool selected{};

    [[nodiscard]] constexpr
    bool isAbove(float x, float y) const {
        return x >= pos.x && y >= pos.y
               && x < pos.x + size.x && y < pos.y + size.y;
    }


    [[nodiscard]] constexpr
    bool isWithin(float xTL, float yTL, float xBR, float yBR) const {
        return xTL <= pos.x + .5*size.x && yTL <= pos.y + .5*size.y
               && xBR >= pos.x + .5*size.x && yBR >= pos.y + .5*size.y;
    }
};
