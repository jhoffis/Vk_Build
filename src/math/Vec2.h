#pragma once

#include "Vec3.h"
#include "math_stuff.h"

struct Vec2 {
    float
            x{0},
            y{0};

    constexpr Vec2 operator-(const Vec3& other) const {
        return Vec2{
                x - other.x,
                y - other.y
        };
    }

    constexpr void operator*=(const float& other) {
        x *= other;
        y *= other;
    }

    [[nodiscard]] constexpr float magnitude() const {
        return MyMath::fast_sqrt((x*x) + (y*y));
    }

    constexpr void normalize() {
        auto magn = magnitude();
        x /= magn;
        y /= magn;
    }
};