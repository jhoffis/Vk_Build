//
// Created by Jens Benz on 22.10.2022.
//
#include "gra_texture.h"
#include "gra_memory_utils.h"
#include "src/vk/gra_setup.h"
#include <stb_image.h>
#include <stdexcept>
#include <filesystem>


namespace Texture {

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    struct ImageData {
        int w;
        int h;
        int comp;
        unsigned char *image;
    };


    /*
     * Helper methods
     */

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {

        // One of the most common ways to perform layout transitions is using an image memory barrier.
        VkCommandBuffer commandBuffer = Gra::beginSingleTimeCommands();
        {
            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;

            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

            barrier.image = image;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            VkPipelineStageFlags sourceStage;
            VkPipelineStageFlags destinationStage;

            if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            } else {
                throw std::invalid_argument("unsupported layout transition!");
            }

            vkCmdPipelineBarrier(
                    commandBuffer,
                    sourceStage, destinationStage,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &barrier
            );
        }
        Gra::endSingleTimeCommands(commandBuffer);
    }

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
        VkCommandBuffer commandBuffer = Gra::beginSingleTimeCommands();
        {
            VkBufferImageCopy region{};
            region.bufferOffset = 0;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;

            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount = 1;

            region.imageOffset = {0, 0, 0};
            region.imageExtent = {
                    width,
                    height,
                    1
            };

            vkCmdCopyBufferToImage(
                    commandBuffer,
                    buffer,
                    image,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1,
                    &region
            );
        }
        Gra::endSingleTimeCommands(commandBuffer);
    }


    /*
     * Creating
     */

    auto loadImage(const char *name) {
        ImageData img{};
        std::string currPath = std::filesystem::current_path().string().append("/res/pics/");
        const char *realPath = reinterpret_cast<const char *>(currPath.append(name).c_str());
        img.image = stbi_load(realPath, &img.w, &img.h, &img.comp, STBI_rgb_alpha);
        // TODO free stb images

        if (img.image == nullptr)
            throw std::runtime_error(std::string("Failed to load texture at ").append(realPath));

        return img;
    }

    void createImage(uint32_t width,
                     uint32_t height,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage &image,
                     VkDeviceMemory &imageMemory
    ) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(Gra::m_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(Gra::m_device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = Gra::findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(Gra::m_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(Gra::m_device, image, imageMemory, 0);
    }

    void createTextureImage() {
        auto texture = loadImage("texture.jpg");
        // The pixels are laid out row by row with 4 bytes per pixel in the case of STBI_rgb_alpha for a total of texWidth * texHeight * 4 values.
        VkDeviceSize imageSize = texture.w * texture.h * 4;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        Gra::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                          stagingBufferMemory);

        void *data;
        vkMapMemory(Gra::m_device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, texture.image, static_cast<size_t>(imageSize));
        vkUnmapMemory(Gra::m_device, stagingBufferMemory);

        stbi_image_free(texture.image);

        createImage(texture.w, texture.h, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    textureImage, textureImageMemory);

        // The next step is to copy the staging buffer to the texture image. This involves two steps:

        transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texture.w), static_cast<uint32_t>(texture.h));

        // To be able to start sampling from the texture image in the shader, we need one last transition to prepare it for shader access:
        transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(Gra::m_device, stagingBuffer, nullptr);
        vkFreeMemory(Gra::m_device, stagingBufferMemory, nullptr);
    }

    GLFWimage createGLFWImage(const char *path) {
        auto img = loadImage(path);

        GLFWimage resultImg;
        resultImg.width = img.w;
        resultImg.height = img.h;
        resultImg.pixels = img.image;

        return resultImg;
    }

    void cleanupTextures() {
        vkDestroyImage(Gra::m_device, textureImage, nullptr);
        vkFreeMemory(Gra::m_device, textureImageMemory, nullptr);
    }

} // Texture