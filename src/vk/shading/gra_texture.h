#ifndef VULKAN_GRA_TEXTURE_H
#define VULKAN_GRA_TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "src/window.h"

namespace Texture {

    extern VkImage textureImage;
    extern VkDeviceMemory textureImageMemory;
    extern VkImageView textureImageView;
    extern VkSampler textureSampler;

    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();
    GLFWimage createGLFWImage(const char *path);
    void cleanupTextures();

} // Texture

#endif //VULKAN_GRA_TEXTURE_H
