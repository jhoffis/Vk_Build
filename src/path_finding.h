#pragma once

#include "rendering/Entity.h"
#include "models/Map.h"

namespace PathFinder {
    bool findPath(Entity entity, Vec2 targetPos, Map::Map map, std::vector<int> &outPath);
}