#pragma once

struct Vec3 {
//    TODO skift over til "std::float32_t" en gang i fremtiden
    float
            x{0},
            y{0},
            z{0};

    Vec3 operator-(const Vec3& other) const {
        return Vec3{
        x - other.x,
        y - other.y,
        z - other.z
        };
    }

    Vec3 operator-() const {
        return Vec3{
                -x, -y, -z
        };
    }

};