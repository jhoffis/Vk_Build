#include "Building.h"
#include "rendering/Model.h"
#include "models/Map.h"

namespace Building {
    std::shared_ptr<Entity> m_hoveringBuilding =
            std::make_shared<Entity>(Entity{.visible = false});

    void startHovering(int i, float wX, float wY) {
        if (isHovering()) stopHovering();
        switch (i) {
            case 0:
                m_hoveringBuilding->sprite = {"house.png"};
                break;
            case 1:
                m_hoveringBuilding->sprite = {"mill.png"};
                break;
        }
        m_hoveringBuilding->visible = true;
        m_hoveringBuilding->pos.x = wX;
        m_hoveringBuilding->pos.y = wY;
        Shaders::m_houseModel.addEntity(m_hoveringBuilding, true);
    }

    void stopHovering() {
        m_hoveringBuilding->visible = false;
        Shaders::m_houseModel.removeEntity(m_hoveringBuilding);
    }

    bool isHovering() {
        return m_hoveringBuilding->visible;
    }

    void updateHovering(float wX, float wY) {
        if (!isHovering()) return;
        m_hoveringBuilding->pos.x = wX;
        m_hoveringBuilding->pos.y = wY;
    }

    void place(float wX, float wY) {
        stopHovering();
        auto mapCoor = Map::worldToMapCoorFloor(wX, wY);
        auto worldCoor = Map::mapToWorldCoordinates(mapCoor);
        Shaders::m_houseModel.spawn(worldCoor.x, worldCoor.y);
        Map::m_map->setInaccessible(true, mapCoor.x, mapCoor.y);
        Map::m_map->setInaccessible(true, mapCoor.x + 1, mapCoor.y);
        Map::m_map->setInaccessible(true, mapCoor.x + 1, mapCoor.y + 1);
        Map::m_map->setInaccessible(true, mapCoor.x, mapCoor.y + 1);
    }

}
