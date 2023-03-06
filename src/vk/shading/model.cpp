#define TINYOBJLOADER_IMPLEMENTATION

#include "model.h"
#include <string>
#include <tiny_obj_loader.h>
#include "gra_vertex.h"
#include "paths.h"
#include <unordered_map>
#include <iostream>

namespace Model {


    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    Mesh loadModel(const char *name) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, Paths::models(name).c_str(), nullptr, true)) {
            throw std::runtime_error(err);
        }

        auto mesh = Mesh{};

        std::unordered_map<Gra::Vertex, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Gra::Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(mesh.vertices.size());
                    mesh.vertices.push_back(vertex);
                }
                mesh.indices.push_back(uniqueVertices[vertex]);
            }
        }
        std::cout << name << " num vertices: " << mesh.vertices.size() << std::endl;

        mesh.vertexBuffer = Gra::createVertexBuffer(mesh.vertices);
        mesh.indexBuffer = Gra::createIndexBuffer(mesh.indices);

        return mesh;
    }
}