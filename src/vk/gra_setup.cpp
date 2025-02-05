
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


#include "gra_setup.h"
#include "src/vk/setup/gra_instance.h"
#include "src/vk/setup/gra_debug.h"
#include "src/vk/setup/gra_physical_device.h"
#include "src/vk/setup/gra_logical_device.h"
#include "src/vk/presentation/gra_surface.h"
#include "src/vk/presentation/gra_swap_chain.h"
#include "src/vk/presentation/gra_image_views.h"
#include "src/vk/pipeline/gra_pipeline.h"
#include "src/vk/pipeline/gra_render_passes.h"
#include "src/vk/drawing/gra_framebuffers.h"
#include "src/vk/drawing/gra_command_buffers.h"
#include "src/vk/drawing/gra_drawing.h"
#include "src/vk/shading/gra_vertex.h"
#include "src/vk/shading/gra_uniform.h"
#include "src/vk/shading/gra_depth.h"
#include <vector>


std::vector<const char *> m_validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};


/*
 * Base
 */
namespace Gra {
    VkDevice m_device;
    VkInstance m_instance;
    std::shared_ptr<VkSurfaceKHR> m_surface;
    std::shared_ptr<VkPhysicalDevice> m_physicalDevice;

    void initVulkan() {
        auto instance = createInstance(
                enableValidationLayers,
                m_validationLayers
        );
        m_instance = *instance;
        setupDebugMessenger(enableValidationLayers, instance);

        auto surface = createSurface(instance);
        m_surface = surface;

        auto physicalDevice = pickPhysicalDevice(instance, surface);
        m_physicalDevice = physicalDevice;

        m_device = createLogicalDevice(
                enableValidationLayers,
                m_validationLayers,
                physicalDevice
        );

        createSwapChain();
        createImageViews();
        createRenderPasses();
        createCommandPool();
        createDepthResources();
        createFramebuffers();
        Texture::createTextureSampler();

        Drawing::createSyncObjects();

#ifdef RMDEV
        Raster::compilePipelines(false);
#endif
    }


    void cleanup() {
        cleanupSwapChain();

        Texture::cleanupTextures();

        cleanupVertex();

        destroyRenderPasses();
        Drawing::cleanupSyncObjects();
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);

        vkDestroyDevice(m_device, nullptr);

        cleanDebug(enableValidationLayers, m_instance);

        vkDestroySurfaceKHR(m_instance, *m_surface, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }

}