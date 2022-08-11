
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
#include "src/vk/presentation/gra_presentation.h"
#include <vector>

std::vector<const char *> m_validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};


/*
 * Base
 */
std::shared_ptr<VkInstance> m_instance;
VkDevice m_device;
VkSurfaceKHR m_surface;

void Gra::initVulkan() {
    m_instance = Gra::createInstance(
            enableValidationLayers,
            m_validationLayers
    );
    Gra::setupDebugMessenger(enableValidationLayers, m_instance);

    m_surface = Gra::createSurface(m_instance);

    auto physicalDevice = Gra::pickPhysicalDevice(m_instance);
    m_device = Gra::createLogicalDevice(
            enableValidationLayers,
            m_validationLayers,
            physicalDevice
    );
}


void Gra::cleanup() {
    vkDestroyDevice(m_device, nullptr);
    Gra::cleanDebug(enableValidationLayers, m_instance);

    vkDestroySurfaceKHR(*m_instance, m_surface, nullptr);
    vkDestroyInstance(*m_instance, nullptr);
    vkDestroyInstance(*m_instance, nullptr);
}

