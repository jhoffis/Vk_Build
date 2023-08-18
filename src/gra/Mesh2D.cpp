#include "Mesh2D.h"

constexpr auto basedSize = 2160.0f; // Best quality for 4k monitors

void Mesh2D::init(float width, float height) {
    worldWidth = width / basedSize;
    worldHeight = height / basedSize;
    vertices[0] = {{0.0f, -worldHeight, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; // top right
    vertices[1] = {{-worldWidth, -worldHeight, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}; // top left
    vertices[2] = {{-worldWidth, 0.0f,  0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}; // bot left
    vertices[3] = {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}; //bot right
}
