#define TINYOBJLOADER_IMPLEMENTATION

#include "model.h"
#include <string>
#include <tiny_obj_loader.h>

namespace Model {


    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    const std::string MODEL_PATH = "models/viking_room.obj";

    Mesh* loadModel(const char *name) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
            throw std::runtime_error(warn + err);
        }
    }
}