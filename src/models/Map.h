#pragma once


#include <vector>
#include <memory>
#include "math/Vec2.h"

namespace Map {

    static const float tileSize = 0.125;

    struct Map {
        const int xy;
        std::vector<int> map{};
        Vec2 indexToWorld(int i) const;

        int mapCoorToIndex(Vec2 vec2);
        bool isMapCoorInaccessible(Vec2 vec2);
    };
    extern std::shared_ptr<Map> m_map;

    Vec2 worldToMapCoordinates(double x, double y);
    Vec2 worldToMapCoorFloor(double x, double y);
    Vec2 worldToMapCoordinates(Vec2 vec2);
    Vec2 mapToWorldCoordinates(Vec2 vec2);

    void createMap(int xy);
    void createVisual(int xy);
    void destroy();

}
