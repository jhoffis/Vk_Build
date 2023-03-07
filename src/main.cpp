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
#include "camera.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Window::createWindow(false, false);
    Gra::initVulkan();
    
    static Camera::Cam camera;

    static size_t x = 0;
    static size_t y = 0;

    // scenes.emplace_back(0); // feiler om scenes er const / constexpr av en eller annen grunn

    glfwSetKeyCallback(Window::m_window, [](auto window, auto key, auto scancode, auto action, auto mods) {
        Camera::inputMovement(&camera, key, action != GLFW_RELEASE);
    });

    glfwSetMouseButtonCallback(Window::m_window, [](auto window, auto button, auto action, auto mods) {
        // mouseButtonInput(&scenes[currentScene], button, action, x, y);
    });

    glfwSetCursorPosCallback(Window::m_window, [](auto window, auto xpos, auto ypos) {
        // x = xpos;
        // y = ypos;
        // mousePosInput(&scenes[currentScene], x, y);
    });

    for(;;)
    {
        if (Window::shouldClose()) {
            //tickThread.join();
            break;
        }
        glfwPollEvents();
        
        Camera::updateMovement(&camera);

        Gra::drawFrame();
    }

    Gra::cleanup();
    Window::destroyWindow();
    return 0;
}
