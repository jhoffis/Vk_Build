#include <stdexcept>
#include <vector>
#include "gra_surface.h"


std::shared_ptr<VkSurfaceKHR> Gra::createSurface(std::shared_ptr<VkInstance>& instance) {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(*instance, Window::m_window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    return std::make_shared<VkSurfaceKHR>(surface);
}
