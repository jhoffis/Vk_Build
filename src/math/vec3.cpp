#include "vec3.h"

constexpr void Math::Vec3::operator+(const Vec3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
}
