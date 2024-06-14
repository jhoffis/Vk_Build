#pragma once

#include "math/Vec3.h"
#include "math/Vec2.h"

struct Entity {
    Vec3 pos{};
    Vec2 size{};
    bool visible{true};
    bool selected{};
    std::string sprite{};

    [[nodiscard]] constexpr
    bool isAbove(float x, float y) const {
        return x >= pos.x && y >= pos.y
               && x < pos.x + size.x && y < pos.y + size.y;
    }


    [[nodiscard]] constexpr
    bool isWithin(float xBL, float yBL, float xTR, float yTR) const {
//        return xTL <= pos.x + .5*size.x && yTL <= pos.y + .5*size.y
//               && xBR >= pos.x + .5*size.x && yBR >= pos.y + .5*size.y;
        auto blX = xBL <= pos.x + size.x; // bottom left is at top right
        auto blY = yBL <= pos.y + size.y;
        auto trX = xTR >= pos.x; // top right is at bottom left
        auto trY = yTR >= pos.y;

        return blX && blY && trX && trY;
    }

    constexpr Vec2 centerPoint(Vec3 pos, Vec2 size) {
        return {pos.x + .5f*size.x ,pos.y + .5f*size.y};
    }

    constexpr Vec2 feetPoint(Vec3 pos, Vec2 size) {
        return {0,0};
    }

};
