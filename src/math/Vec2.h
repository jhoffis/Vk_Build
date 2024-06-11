#pragma once

#include <complex>
#include "math_stuff.h"
#include "Vec3.h"

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

    constexpr bool operator>(const Vec2& other) const {
        return (std::abs(x)+std::abs(y)) > (std::abs(other.x)+std::abs(other.y));
    }

    constexpr bool operator==(const Vec2& other) const {
        return x==other.x && y==other.y;
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