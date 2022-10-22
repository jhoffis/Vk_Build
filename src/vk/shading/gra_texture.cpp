//
// Created by Jens Benz on 22.10.2022.
//
#include "gra_texture.h"
#include <stb_image.h>
#include <stdexcept>
#include <filesystem>

namespace Texture {


    GLFWimage createGLFWImage(const char *path) {
        int w;
        int h;
        int comp;
        std::string currPath = std::filesystem::current_path().string().append("/res/");
        const char *realPath = reinterpret_cast<const char *>(currPath.append(path).c_str());
        unsigned char *image = stbi_load(realPath, &w, &h, &comp, STBI_rgb_alpha);
        // TODO free stb images

        if (image == nullptr)
            throw std::runtime_error(std::string("Failed to load texture at ").append(realPath));

        GLFWimage resultImg;
        resultImg.width = w;
        resultImg.height = h;
        resultImg.pixels = image;

        return resultImg;
    }

} // Texture