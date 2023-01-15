#ifndef VULKAN_GRA_TEXTURE_H
#define VULKAN_GRA_TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "src/window.h"

namespace Texture {

    void createTextureImage();
    void createTextureImageView();
    GLFWimage createGLFWImage(const char *path);
    void cleanupTextures();

} // Texture

#endif //VULKAN_GRA_TEXTURE_H
