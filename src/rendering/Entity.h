#pragma once

#include "math/Vec3.h"
#include "math/Vec2.h"
#include "rendering/Mesh2D.h"
#include <vector>

struct Entity {
    Vec3 pos{};
    Vec2 scale{1, 1}; // TODO make work
    Mesh2D* mesh{};
    bool visible{true};
    bool selected{};
    std::vector<std::string> sprite{};

    [[nodiscard]] constexpr
    bool isAbove(float x, float y) const {
        return x >= pos.x && y >= pos.y
               && x < pos.x + mesh->worldWidth && y < pos.y + mesh->worldHeight;
    }


    [[nodiscard]] constexpr
    bool isWithin(float xBL, float yBL, float xTR, float yTR) const {
//        return xTL <= pos.x + .5*size.x && yTL <= pos.y + .5*size.y
//               && xBR >= pos.x + .5*size.x && yBR >= pos.y + .5*size.y;
        auto blX = xBL <= pos.x + mesh->worldWidth; // bottom left is at top right
        auto blY = yBL <= pos.y + mesh->worldHeight;
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
