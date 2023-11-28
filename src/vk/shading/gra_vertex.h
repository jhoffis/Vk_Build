#pragma once

#include "gra_elems/Mesh2D.h"
#include "gra_elems/Vertex.h"
#include <vector>

namespace Gra {

    struct UniformBufferObject {
        alignas(8) Vec3 pos;
        alignas(4) float aspect;
//        alignas(16) glm::mat4 model{};
//        alignas(16) glm::mat4 view{};
//        alignas(16) glm::mat4 proj{};
    };

    void createVertexBuffer(Mesh2D *mesh);
    void createIndexBuffer(Mesh2D *mesh);

    void cleanupVertex();
}
