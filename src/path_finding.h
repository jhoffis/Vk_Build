#pragma once

#include "rendering/Entity.h"
#include "models/Map.h"

namespace PathFinder {
    bool findPath(Vec2 startPos, Vec2 targetPos, Map::Map map, std::vector<int> &outPath);

    void convertMapPathToWorldPath(Map::Map &map,
                                   std::vector<int> &inPath,
                                   std::vector<Vec2> &outPath);
}