#include "Building.h"
#include "rendering/Model.h"
#include "models/Map.h"

namespace Building {
    std::shared_ptr<Entity> m_hoveringBuilding =
            std::make_shared<Entity>(Entity{.visible = false});

    void startHovering(float wX, float wY) {
        if (isHovering()) stopHovering();
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
    }

}