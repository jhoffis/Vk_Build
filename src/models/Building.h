#pragma once

#include "math/Vec2.h"
#include "math/Vec3.h"
namespace Building {

    struct HouseUBO {
        alignas(16) Vec3 pos;
        alignas(4) float aspect{};
        alignas(4) int selected;
        alignas(8) Vec2 dimensions;
    };

    void init();
    void startHovering(int i, float wX, float wY);
    void stopHovering();
    bool isHovering();
    void updateHovering(float wX, float wY);
    void place(float wX, float wY);
}
