//
// Created by Jens Benz on 22.10.2022.
//
#include "gra_texture.h"
#include "gra_memory_utils.h"
#include "src/vk/gra_setup.h"
#include "src/vk/presentation/gra_image_views.h"
#include <stb_image.h>
#include <stdexcept>
#include <filesystem>


namespace Texture {

    struct TextureData {
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
    };

    std::vector<TextureData> texsToClean{};

//    VkImage m_textureImage;
//    VkDeviceMemory m_textureImageMemory;
//    VkImageView m_textureImageView; // Points to textureImage
    VkSampler m_textureSampler; // Holds information about how to render like e.g. filters

    /*
     * Helper methods
     */
    
    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

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
            if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

                if (hasStencilComponent(format)) {
                    barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
                }
            } else {
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            }
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
            } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

                sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
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
    ImageData loadImage(const char *name) {
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

    void createTextureImage(ImageData &texture, TextureData *texData) {
        // The pixels are laid out row by row with 4 bytes per pixel in the case of STBI_rgb_alpha for a total of texWidth * texHeight * 4 values.
        VkDeviceSize imageSize = texture.w * texture.h * 4;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        Gra::createBuffer(imageSize,
                          VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                          stagingBuffer,
                          stagingBufferMemory);

        void *data;
        vkMapMemory(Gra::m_device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, texture.image, static_cast<size_t>(imageSize));
        vkUnmapMemory(Gra::m_device, stagingBufferMemory);

        stbi_image_free(texture.image);

        createImage(texture.w, texture.h, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    texData->textureImage, texData->textureImageMemory);

        // The next step is to copy the staging buffer to the texture image. This involves two steps:

        transitionImageLayout(texData->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        copyBufferToImage(stagingBuffer, texData->textureImage, static_cast<uint32_t>(texture.w), static_cast<uint32_t>(texture.h));

        // To be able to start sampling from the texture image in the shader, we need one last transition to prepare it for shader access:
        transitionImageLayout(texData->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(Gra::m_device, stagingBuffer, nullptr);
        vkFreeMemory(Gra::m_device, stagingBufferMemory, nullptr);
    }

    VkImageView createTexture(ImageData &img) {
        TextureData data{};
        createTextureImage(img, &data);
        data.textureImageView = Gra::createImageView(data.textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
        texsToClean.emplace_back(data);
        return data.textureImageView;
    }

    /*
    *   NN, bilinear Filtering, anisotropic filtering, addressing mode
    */
    void createTextureSampler()
    {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(*Gra::m_physicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo{
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter = VK_FILTER_NEAREST, // specify how to interpolate texels that are magnified or minified
            .minFilter = VK_FILTER_NEAREST,

            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,

            // UVW instead of XYZ
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            
            .mipLodBias = 0.0f,

            .anisotropyEnable = VK_TRUE,
            .maxAnisotropy = properties.limits.maxSamplerAnisotropy,

            // true then texels will first be compared to a value, and the result of that comparison is used in filtering operations
            .compareEnable = VK_FALSE,
            .compareOp = VK_COMPARE_OP_ALWAYS,
            
            .minLod = 0.0f,
            .maxLod = 0.0f,
            
            .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,

            // specifies which coordinate system you want to use to address texels in an image.
            // True is 0->texHeight/Width, false is 0->1
            .unnormalizedCoordinates = VK_FALSE,
        };

        if (vkCreateSampler(Gra::m_device, &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
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
        vkDestroySampler(Gra::m_device, m_textureSampler, nullptr);
        for (auto tex : texsToClean) {
            vkDestroyImageView(Gra::m_device, tex.textureImageView, nullptr);
            vkDestroyImage(Gra::m_device, tex.textureImage, nullptr);
            vkFreeMemory(Gra::m_device, tex.textureImageMemory, nullptr);
        }
    }

} // Texture