#include "gra_depth.h"
#include "src/vk/gra_setup.h"
#include "src/vk/setup/gra_physical_device.h"
#include "src/vk/presentation/gra_image_views.h"
#include "src/vk/presentation/gra_swap_chain.h"
#include "gra_texture.h"
#include <vector>

namespace Gra {

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkFormat findDepthFormat() {
        return findSupportedFormat(
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    /*
    It should have the same resolution as the color attachment, defined by the swap chain extent, 
    an image usage appropriate for a depth attachment, optimal tiling and device local memory

    Unlike the texture image, we don't necessarily need a specific format, because we won't be directly accessing the texels from the program
    */
    void createDepthResources()
    {
        VkFormat depthFormat = findDepthFormat();

        Texture::createImage(
            Gra::m_swapChainExtent.width, 
            Gra::m_swapChainExtent.height, 
            depthFormat,
            VK_IMAGE_TILING_OPTIMAL, 
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            depthImage, 
            depthImageMemory
        );
        depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        Texture::transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }

    void destroyDepth(VkDevice device)
    {
        vkDestroyImageView(device, depthImageView, nullptr);
        vkDestroyImage(device, depthImage, nullptr);
        vkFreeMemory(device, depthImageMemory, nullptr);
    }
} // Gra