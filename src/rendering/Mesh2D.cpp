#include "Mesh2D.h"

constexpr auto basedSize = 128.f * 8; // Best quality for 4k monitors (no not anymore)

void Mesh2D::init(float width, float height, uint32_t numInstances) {
    worldWidth = width / basedSize;
    worldHeight = height / basedSize;
    vertices[0] = {
            {0.0f, -worldHeight, 0.0f},
            {1.0f, 0.0f,         0.0f},
            {0.0f, 0.0f}
    }; // top right
    vertices[1] = {{worldWidth, -worldHeight, 0.0f},
                   {0.0f,        1.0f,         0.0f},
                   {1.0f,        0.0f}}; // top left
    vertices[2] = {{worldWidth, 0.0f, 0.0f},
                   {0.0f,        0.0f, 1.0f},
                   {1.0f,        1.0f}}; // bot left
    vertices[3] = {{0.0f, 0.0f, 0.0f},
                   {1.0f, 1.0f, 1.0f},
                   {0.0f, 1.0f}}; //bot right

    instanceData.resize(numInstances);
    for (uint32_t i = 0; i < numInstances; i++) {
        instanceData[i] = InstanceData{.index = i};
    }
    instanceData[0].pos = {0, 0}; 
    //instanceData[1].pos = {1, 1}; // FIXME something is COMPLETELY off with this one. Why is it going after instance number? Like for instance therer is only one instance of selection box
    

}
