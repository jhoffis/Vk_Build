#ifndef VULKAN_GRA_TEXTURE_H
#define VULKAN_GRA_TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "src/window.h"

namespace Texture {

    extern VkSampler m_textureSampler;


    struct ImageData {
        int w;
        int h;
        int comp;
        unsigned char *image;
    };

    void createImage(uint32_t width,
                     uint32_t height,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage &image,
                     VkDeviceMemory &imageMemory
    );

    ImageData loadImage(const char *name);
    void createTextureSampler();
    GLFWimage createGLFWImage(const char *path);
    VkImageView createTexture(ImageData &img);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void cleanupTextures();

} // Texture

#endif //VULKAN_GRA_TEXTURE_H
