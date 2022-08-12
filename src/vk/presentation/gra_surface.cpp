#include <stdexcept>
#include "gra_presentation.h"
#include <cassert>


std::shared_ptr<VkSurfaceKHR> Gra::createSurface(std::shared_ptr<VkInstance>& instance) {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(*instance, Window::getWindow(), nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    return std::make_shared<VkSurfaceKHR>(surface);
}