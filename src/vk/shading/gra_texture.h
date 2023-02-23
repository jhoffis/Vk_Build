#ifndef VULKAN_GRA_TEXTURE_H
#define VULKAN_GRA_TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "src/window.h"

namespace Texture {

    extern VkImage textureImage;
    extern VkDeviceMemory textureImageMemory;
    extern VkImageView textureImageView;
    extern VkSampler textureSampler;

    void createImage(uint32_t width,
                     uint32_t height,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage &image,
                     VkDeviceMemory &imageMemory
    );

    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();
    GLFWimage createGLFWImage(const char *path);
    void cleanupTextures();

} // Texture

#endif //VULKAN_GRA_TEXTURE_H
