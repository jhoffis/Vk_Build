#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "src/window.h"

namespace Texture {

    extern VkSampler textureSampler;

    struct TexData {
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
    };

    void createImage(uint32_t width,
                     uint32_t height,
                     uint32_t mipLevels, 
                     VkSampleCountFlagBits numSamples,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage &image,
                     VkDeviceMemory &imageMemory
    );

    TexData createTextureImage(const char *name);
    void createTextureSampler();
    GLFWimage createGLFWImage(const char *path);

    void transitionImageLayout(
        VkImage image, 
        VkFormat format, 
        VkImageLayout oldLayout, 
        VkImageLayout newLayout, 
        uint32_t mipLevels
    );
    
    void cleanupTextures();

} // Texture
