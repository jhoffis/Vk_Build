#include "path_finding.h"
#include <vector>

bool PathFinder::findPath(Vec2 startPos, Vec2 targetPos, Map::Map map, std::vector<int> &outPath) {
    // List containing which path point to check next. The first elements are the closest to Start which ensures shortest path
    std::vector<int> checkList(map.map.size(), -1);
    int checkIndex = 0;
    int checkPlacementIndex = 0;

    // History for traversing back to where you started
    std::vector<int> paths(checkList);

    int x = static_cast<int>(std::floor(startPos.x));
    int y = static_cast<int>(std::floor(startPos.y));
    int targetX = static_cast<int>(std::floor(targetPos.x));
    int targetY = static_cast<int>(std::floor(targetPos.y));
    if (x == targetX && y == targetY) return false;

    auto mapDimension = map.xy;
    int startIndex = x + y * mapDimension;
    int targetIndex = targetX + targetY * mapDimension;
    paths[startIndex] = startIndex;
    int pathIndex = startIndex;

    // right, left, up, down, right up, right down, left up, left down
    int_fast8_t horizontalCheck[8] = {1, -1, 0, 0,
                                      1, 1, -1, -1};
    int_fast8_t verticalCheck[8] = {0, 0, 1, -1,
                                    1, -1, 1, -1};

    while (pathIndex != targetIndex) {

        bool closedTile[4];

        // Check this points neighbors
        for (auto i = 0; i < 8; i++) {
            unsigned int xNeighbor = x + horizontalCheck[i];
            unsigned int yNeighbor = y + verticalCheck[i];
            if (xNeighbor >= mapDimension || yNeighbor >= mapDimension) {
                continue;
            }

            auto index = xNeighbor + yNeighbor * mapDimension;
            auto inaccessible = map.map[index] == 0;
            auto alreadyChecked = paths[index] >= 0;
            if (i < 4) {
                closedTile[i] = inaccessible;
            } else if (i == 4 && (closedTile[0] && closedTile[2])) {
                continue;
            } else if (i == 5 && (closedTile[0] && closedTile[3])) {
                continue;
            } else if (i == 6 && (closedTile[1] && closedTile[2])) {
                continue;
            } else if (i == 7 && (closedTile[1] && closedTile[3])) {
                continue;
            }

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

    std::vector<int> tempOutPath{};
    while (pathIndex != startIndex) {
        tempOutPath.emplace_back(pathIndex);
        pathIndex = paths[pathIndex];
    }
    tempOutPath.emplace_back(startIndex);
    std::reverse(tempOutPath.begin(), tempOutPath.end());

    int lastDirection{};
    for (unsigned int i = 0; i < tempOutPath.size() - 1; i++) {
        auto direction = tempOutPath[i + 1] - tempOutPath[i]; // TODO error when going to same tile as the one you stand on.
        if (direction != lastDirection && i != 0) {
            outPath.emplace_back(tempOutPath[i]);
        }
        lastDirection = direction;
    }
    outPath.emplace_back(tempOutPath[tempOutPath.size() - 1]);

    return true;
}

void PathFinder::convertMapPathToWorldPath(Map::Map &map,
                                           std::vector<int> &inPath,
                                           std::vector<Vec2> &outPath) {
    outPath.clear();
    for (int i = 0; i < inPath.size(); i++) {
        outPath.emplace_back(map.indexToWorld(inPath[i]));
    }
}
