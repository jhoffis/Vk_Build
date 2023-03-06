#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32


// Lage et kattespill som er idle in essence MEN som kan fungere som et slukende tradisjonelt spill - F.eks "Mr. Erik Loves Pussy"

#include "src/vk/gra_setup.h"
#include <iostream>
#include "src/vk/drawing/gra_drawing.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Window::createWindow(false, false);
    Gra::initVulkan();

    static bool running = true;
    while(running)
    {
        if (glfwWindowShouldClose(Window::m_window)) {
            running = false;
            //tickThread.join();
            break;
        }
        glfwPollEvents();
        Gra::drawFrame();
    }

    Gra::cleanup();
    Window::destroyWindow();
    return 0;
}
