#pragma once

#include <string>
#include "hash.h"

namespace Math {

    struct Vec2 {
        double x{0.0};
        double y{0.0};

        constexpr void operator+=(const Vec2& other){
            x += other.x;
            y += other.y;
        }

        // https://www.matematikk.net/side/Skalarprodukt
        constexpr void operator*=(const Vec2& other){
            x *= other.x;
            y *= other.y;
        }

        // https://en.wikipedia.org/wiki/Divergence
        constexpr void operator/=(const Vec2& other){
            x /= other.x;
            y /= other.y;
        }

        constexpr bool operator==(const Vec2& other) const {
            return
                x == other.x &&
                y == other.y;
        }

        constexpr void operator*=(const double &scalar){
            x *= scalar;
            y *= scalar;
        }

        constexpr size_t hash() const {
            size_t seed = 0;
            Math::hashCombine(seed, x);
            Math::hashCombine(seed, y);
            return seed;
        }

        std::string toString();
    };
}