#include "Map.h"
#include "rendering/Model.h"
#include "shaders/Shaders.h"

std::shared_ptr<Map::Map> Map::m_map{};

void Map::createMap(int xy) {
    m_map = std::make_shared<Map>(Map{.xy = xy});
    m_map->map.resize(xy*xy);
    std::fill(m_map->map.begin(), m_map->map.end(), 1);
}

void Map::createVisual(int xy) {
    const uint32_t count = xy*xy;
    Shaders::m_grassModel.box = Gra_desc::createDescriptorBox(1, {
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
            .textureNames = {"grass.png"}
            },
            });
    
    auto img = Texture::loadImage("grass.png");
    Shaders::m_grassModel.init(count, img.w, img.h);
    for (auto x = 0; x < xy; x++) {
        for (auto y = 0; y < xy; y++) {
            float X = static_cast<float>(x); 
            float Y = static_cast<float>(y); 
            Shaders::m_grassModel.spawn({
                    (X - Y) * .5f*static_cast<float>(img.w) / 128.0f,
                    (X + Y) * .5f*static_cast<float>(img.h) / 128.0f},
                    "grass.png");
        }
    }
}

void Map::destroy() {
    Shaders::m_grassModel.destroy();
}

Vec2 Map::worldToMapCoordinates(double x, double y) {
    return Vec2(x / tileSize, y / tileSize);
}

Vec2 Map::worldToMapCoorFloor(double x, double y) {
    return Vec2(std::floor(x / tileSize), std::floor(y / tileSize));
}

Vec2 Map::worldToMapCoordinates(const Vec2 vec2) {
    return Vec2(vec2.x / tileSize, vec2.y / tileSize);
}

Vec2 Map::mapToWorldCoordinates(Vec2 vec2) {
    return Vec2(vec2.x * tileSize, vec2.y * tileSize);
}

Vec2 Map::Map::indexToWorld(int i) const {
    return Vec2(
            static_cast<float>(i % xy) * tileSize,
            static_cast<float>(i / xy) * tileSize
    );
}

int Map::Map::mapCoorToIndex(Vec2 vec2) {
    return static_cast<int>(std::floor(vec2.x)) + static_cast<int>(std::floor(vec2.y)) * xy;
}

int Map::Map::mapCoorToIndex(int x, int y) {
    return x + y * xy;
}

bool Map::Map::isMapCoorInaccessible(Vec2 vec2) {
    return map[mapCoorToIndex(vec2)] == 0;
}

void Map::Map::setInaccessible(bool inaccessible, int x, int y) {
    map[mapCoorToIndex(x, y)] = !inaccessible;
}

