#include "vec3.h"

namespace Math {
    constexpr void Vec3::operator+(const Vec3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    std::string Vec3::toString() {
        return  "[" + std::to_string(x) + ", " + std::to_string(x) + ", " + std::to_string(x) + "]";
    }
}