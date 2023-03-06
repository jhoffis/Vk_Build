#pragma once
#include <vector>
#include "gra_vertex.h"

namespace Model {

    struct Mesh {
        std::vector<Gra::Vertex> vertices;
        std::vector<uint32_t> indices;     // // TODO lag en check på int størrelse i forhold til attributeDescriptions[1].format osv fordi om du definerer feil int størrelse så vil den ikke klage men heller ikke fungere!
        VkBuffer vertexBuffer;
        VkBuffer indexBuffer;
    };

    Mesh loadModel(const char *name);
}