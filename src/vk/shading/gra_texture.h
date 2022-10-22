#ifndef VULKAN_GRA_TEXTURE_H
#define VULKAN_GRA_TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "src/window.h"

namespace Texture {

    GLFWimage createGLFWImage(const char *path);

} // Texture

#endif //VULKAN_GRA_TEXTURE_H
