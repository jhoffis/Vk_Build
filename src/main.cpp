#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32

#include "src/vk/gra_setup.h"
#include <iostream>
#include "src/vk/drawing/gra_drawing.h"
#include "camera.h"
#include <thread>


int main() {
    Window::createWindow(false, false);
    Gra::initVulkan();

    static Camera::Cam cam{};
    cam.fov = 80.0f;
    cam.aspect = 2.0f;
    cam.nearPlane = 0.1f;
    cam.farPlane = 1000.0f;
    cam.position = glm::vec3(2.0f, 2.0f, 2.0f);
    cam.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
    cam.upOrientation = glm::vec3(0.0f, 1.0f, 0.0f);
    std::cout << "x: " << cam.rotation.x << "z: " << cam.rotation.z << std::endl;
    cam.updateView();
    std::cout << "x: " << cam.rotation.x << "z: " << cam.rotation.z << std::endl;
    cam.updateProjection();

    static size_t x = 0;
    static size_t y = 0;

    // scenes.emplace_back(0); // feiler om scenes er const / constexpr av en eller annen grunn

    glfwSetKeyCallback(Window::m_window, [](auto window, auto key, auto scancode, auto action, auto mods) {
        cam.inputMovement(key, action != GLFW_RELEASE);
    });

    glfwSetMouseButtonCallback(Window::m_window, [](auto window, auto button, auto action, auto mods) {
    });

    glfwSetCursorPosCallback(Window::m_window, [](auto window, auto xpos, auto ypos) {
        x = xpos;
        y = ypos;
        cam.inputMouse(static_cast<float>(xpos), static_cast<float>(ypos));
    });

    glfwSetInputMode(Window::m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    for(;;)
    {
        if (Window::shouldClose()) {
            //tickThread.join();
            break;
        }
        glfwPollEvents();
        
        cam.updateMovement();
        cam.updateView();

        Gra::drawFrame(cam);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    Gra::cleanup();
    Window::destroyWindow();
    return 0;
}
