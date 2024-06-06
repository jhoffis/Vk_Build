#pragma once

#include "rendering/Entity.h"
#include "models/Map.h"

namespace PathFinder {
    bool findPath(Vec2 startPos, Vec2 targetPos, Map::Map map, std::vector<int> &outPath);

    void convertMapPathToWorldPath(Map::Map &map,
                                   std::vector<int> &inPath,
                                   std::vector<Vec2> &outPath);

    std::vector<Vec2> getLineTilesDDA(Vec2 start, Vec2 end);

    int nextClearLineTilesDDA(Map::Map &map,
                              const Vec2 &pos,
                              std::vector<Vec2> &inPath,
                              int startingIndex);
}