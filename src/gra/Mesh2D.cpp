#include "Mesh2D.h"

constexpr auto basedSize = 2160.0f; // Best quality for 4k monitors

void Mesh2D::init(float width, float height) {
    auto calculatedWidth = -(width / basedSize);
    auto calculatedHeight = -(height / basedSize);
    vertices[0] = {{calculatedWidth, calculatedHeight, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; // top right
    vertices[1] = {{0.0f, calculatedHeight, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}; // top left
    vertices[2] = {{0.0f, 0.0f,  0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}; // bot left
    vertices[3] = {{calculatedWidth, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}; //bot right
}
