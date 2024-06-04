#include "path_finding.h"
#include <vector>
#include <cstdint>
#include <algorithm>

#include <iostream>
#include <chrono>

bool PathFinder::findPath(Entity entity, Vec2 targetPos, Map::Map map, std::vector<int> &outPath) {
        // List containing which path point to check next. The first elements are the closest to Start which ensures shortest path
        std::vector<int> checkList(map.map.size(), -1);
        int checkIndex = 0;
        int checkPlacementIndex = 0;

        // History for traversing back to where you started
        std::vector<int> paths(checkList);

        int x = static_cast<int>(entity.pos.x);
        int y = static_cast<int>(entity.pos.y);
        auto mapDimension = map.xy;
        int startIndex = x + y * mapDimension;
        int targetIndex = static_cast<int>(targetPos.x) + static_cast<int>(targetPos.y) * mapDimension;
        paths[startIndex] = startIndex;
        int pathIndex = startIndex;

        int_fast8_t horizontalCheck[4] = {1, -1, 0, 0};
        int_fast8_t verticalCheck[4] = {0, 0, 1, -1};

        while (pathIndex != targetIndex) {

            // Check this points neighbors
            for (int_fast8_t i = 0; i < 4; i++) {
                unsigned int xNeighbor = x + horizontalCheck[i];
                unsigned int yNeighbor = y + verticalCheck[i];
                if (xNeighbor >= mapDimension || yNeighbor >= mapDimension) {
                    continue;
                }

                auto index = xNeighbor + yNeighbor * mapDimension;
                auto inaccessible = map.map[index] == 0;
                auto alreadyChecked = paths[index] >= 0;
                if (inaccessible || alreadyChecked) {
                    continue;
                }
                paths[index] = pathIndex;

                // Point or index is traversable, save it for later checking
                checkList[checkPlacementIndex] = static_cast<int>(index);
                checkPlacementIndex++;
            }

            // Ready next point to check                                                    #include <vector>
            if (checkIndex < checkPlacementIndex) {
                pathIndex = checkList[checkIndex];
                checkIndex++;

                x = pathIndex % mapDimension;
                y = pathIndex / mapDimension;
            } else {
                // No more routes to check
                return false;
            }
        }

        while (pathIndex != startIndex) {
            outPath.emplace_back(pathIndex);
            pathIndex = paths[pathIndex];
        }
        std::reverse(outPath.begin(), outPath.end());

        return true;
}
