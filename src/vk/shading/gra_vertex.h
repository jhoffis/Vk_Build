#pragma once

#include "src/window.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "math/mat.h"
#include <array>
#include <vector>

namespace Gra {

    struct Vertex {
        Math::Vec3 pos;
        Math::Vec3 color;
        Math::Vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{
                    .binding = 0,
                    .stride = sizeof(Vertex),
                    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            };

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
        
        // For hashing and removing duplicate vertices
        constexpr bool operator==(const Vertex& other) const {
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }
    };

    VkBuffer createVertexBuffer(std::vector<Vertex> vertices);
    VkBuffer createIndexBuffer(std::vector<uint32_t> indices);

    void cleanupVertex();
}


// For hashing and removing duplicate vertices
namespace std {

    template<> struct hash<Gra::Vertex> {
        size_t operator()(Gra::Vertex const& vertex) const {
            return ((vertex.pos.hash() ^
                   (vertex.color.hash() << 1)) >> 1) ^
                   (vertex.texCoord.hash() << 1);
        }
    };
}
