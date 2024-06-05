#pragma once


#include <vector>
#include "math/Vec2.h"

namespace Map {

    static const float tileSize = 0.125;

    struct Map {
        const int xy;
        std::vector<int> map{};
        Vec2 indexToWorld(int i) const;
    };

    Vec2 worldToMapCoordinates(double x, double y);

    Map createMap(int xy);
    void createVisual(int xy);
    void destroy();
}
