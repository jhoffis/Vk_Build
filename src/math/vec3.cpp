#include "vec3.h"

namespace Math {

    std::string Vec3::toString() {
        return  "[" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "]";
    }

}