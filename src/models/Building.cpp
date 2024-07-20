#include "Building.h"
#include "camera.h"
#include "random_util.h"
#include "rendering/Model.h"
#include "models/Map.h"
#include "vk/presentation/gra_swap_chain.h"
#include <cstdint>
#include <iostream>

namespace Building {

    std::shared_ptr<Entity> m_hoveringBuilding = std::make_shared<Entity>(Entity{.visible = false});
    std::unordered_map<uint32_t, BuildingStats> m_buildings{};
    BuildingTypeInfo m_buildingInfos[COUNT];

    void init() {
        uint32_t count = 1;
        Shaders::m_houseModel.box = Gra_desc::createDescriptorBox(2, {
                {
                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .bindingNum = 0,
                .count = count,
                .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                .sizeofUBO = sizeof(Gra::UniformBufferObject), 
                },
                {
                .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .bindingNum = 1,
                .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                .textureNames = {"house.png"}
                },
                });

        // auto img = Texture::loadImage("house.png");
        Shaders::m_houseModel.init(count);
        Shaders::m_houseModel.updateRenderUbo = [](auto uboMem, auto entity, auto index) {
            static_cast<HouseUBO*>(uboMem->uboStruct)[index] = {
                .pos = entity->pos - Camera::m_cam.pos,
                .aspect = Gra::m_swapChainAspectRatio,
                .selected = entity->selected,
                .dimensions = m_buildingInfos[m_buildings[entity->id].type].dimensions,
            };
        };
        Shaders::m_houseModel.initRenderUbo = [](auto uboMem) {
            delete static_cast<HouseUBO *>(uboMem->uboStruct);
            uboMem->uboStruct = new HouseUBO[uboMem->count];
        };

        m_buildingInfos[house] = {.dimensions = {1, 1}};
        m_buildingInfos[mill] = {.dimensions = {2, 3}};
    }

    void startHovering(int i, float wX, float wY) {
        if (isHovering()) stopHovering();
        m_hoveringBuilding->id = 0;
        m_buildings[0] = {
            .type = static_cast<BuildingType>(i),
        };
        switch (i) {
            case 0:
                m_hoveringBuilding->sprite = {"house.png"};
                break;
            case 1:
                m_hoveringBuilding->sprite = {"mill.png"};
                break;
        }
        m_hoveringBuilding->visible = true;
        // m_hoveringBuilding->pos.x = wX;
        // m_hoveringBuilding->pos.y = wY;
        Shaders::m_houseModel.spawn(m_hoveringBuilding);
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
        auto mapCoor = Map::worldToMapCoorFloor(wX, wY);
        auto mapPos = Map::mapToWorldCoordinates(mapCoor);
        m_hoveringBuilding->pos.x = mapPos.x;
        m_hoveringBuilding->pos.y = mapPos.y;
    }

    void place(float wX, float wY) {
        stopHovering();
        auto mapCoor = Map::worldToMapCoorFloor(wX, wY);
        auto entity = Shaders::m_houseModel.spawn({mapCoor.x, mapCoor.y}, m_hoveringBuilding->sprite[0]);
        entity->id = RandomUtil::generateNumber(); 
        auto type = m_buildings[0].type;
        m_buildings[entity->id] = {
            .type = type,
        };
        auto dimensions = m_buildingInfos[type].dimensions; 
        Map::m_map->setInaccessible(true, mapCoor.x, mapCoor.y);
        Map::m_map->setInaccessible(true, mapCoor.x + (dimensions.x - 1), mapCoor.y);
        Map::m_map->setInaccessible(true, mapCoor.x + (dimensions.x - 1), mapCoor.y + (dimensions.y - 1));
        Map::m_map->setInaccessible(true, mapCoor.x, mapCoor.y + (dimensions.y - 1));

        //     Map::m_map->setInaccessible(true, mapCoor.x, mapCoor.y);
        //     Map::m_map->setInaccessible(true, mapCoor.x + 1, mapCoor.y);
        //     Map::m_map->setInaccessible(true, mapCoor.x + 1, mapCoor.y + 1);
        //     Map::m_map->setInaccessible(true, mapCoor.x, mapCoor.y + 1);
        // }
    }

}
