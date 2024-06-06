#include "Building.h"
#include "rendering/Model.h"
#include "models/Map.h"

void Building::spawn(float x, float y) {
    auto mapCoor = Map::worldToMapCoorFloor(x, y);
    auto worldCoor = Map::mapToWorldCoordinates(mapCoor);
    Shaders::m_houseModel.spawn(worldCoor.x, worldCoor.y);
}
