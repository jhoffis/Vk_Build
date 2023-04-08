#include "vec2.h"

namespace Math {

    std::string Vec2::toString() {
        return  "[" + std::to_string(x) + ", " + std::to_string(y) + "]";
    }

}