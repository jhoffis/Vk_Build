
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#include "gra_setup.h"
#include "gra_instance.h"
#include "gra_debug.h"
#include "gra_physical_device.h"
#include "gra_logical_device.h"
#include <vector>

const std::vector<const char *> m_validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};


/*
 * Base
 */


void Gra::initVulkan() {
    auto instance = Gra::createInstance(
            enableValidationLayers,
            m_validationLayers
    );
    Gra::setupDebugMessenger(enableValidationLayers, instance);
    auto physicalDevice = Gra::pickPhysicalDevice(instance);
    Gra::createLogicalDevice(
            enableValidationLayers,
            physicalDevice
    );
}


void Gra::cleanup() {
//    vkDestroyDevice(device, nullptr);
//
//    if (enableValidationLayers) {
//        DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
//    }
//
//    vkDestroyInstance(m_instance, nullptr);
}

