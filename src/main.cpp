#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#ifdef _WIN32
	#define VK_USE_PLATFORM_WIN32_KHR
	#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#define GLFW_INCLUDE_VULKAN

#include "src/vk/gra_setup.h"
#include <iostream>
#include "src/vk/drawing/gra_drawing.h"
#include "camera.h"
#include <thread>
#include "math/mat.h"
#include "math/vec3.h"

int main() {
    Math::Mat mat{};
    std::cout << mat.toString() << std::endl;

    Math::Vec3 v1{
        .x = 4.0f,
        .z = 2.2f
    };
    Math::Vec3 v2{
        .x = 1.0f,
        .y = 6.9f,
        .z = 2.2f
    };
    v1 += v2;
    std::cout << v1.toString() << std::endl;

    Window::createWindow(false, false);
    Gra::initVulkan();

    static Camera::Cam cam{};
/*    cam.fov = 80.0f;
    cam.aspect = 2.0f;
    cam.nearPlane = 0.1f;
    cam.farPlane = 1000.0f;
//    cam.position = Math::Vec3(-0.0f, -0.0f, -0.0f);
//    cam.rotation = std::make_shared<glm::vec3>(glm::vec3(glm::radians(-90.001f), 10.0f, 0.01f));
//    cam.upOrientation = std::make_shared<glm::vec3>(glm::vec3(0.0f, 1.0f, 0.0f));
    std::cout << "x: " << cam.rotation.x << "z: " << cam.rotation.z << std::endl;
    cam.updateView();
    std::cout << "x: " << cam.rotation.x << "z: " << cam.rotation.z << std::endl;
    cam.updateProjection();
    */
//
//    static size_t x = 0;
//    static size_t y = 0;
//
//    // scenes.emplace_back(0); // feiler om scenes er const / constexpr av en eller annen grunn
//
//    glfwSetKeyCallback(Window::m_window, [](auto window, auto key, auto scancode, auto action, auto mods) {
//        cam.inputMovement(key, action != GLFW_RELEASE);
//    });
//
//    glfwSetMouseButtonCallback(Window::m_window, [](auto window, auto button, auto action, auto mods) {
//    });
//
//    glfwSetCursorPosCallback(Window::m_window, [](auto window, auto xpos, auto ypos) {
//        // x = xpos;
//        // y = ypos;
//        // cam.inputMouse(static_cast<float>(xpos), static_cast<float>(ypos));
//    });
//
//    // glfwSetInputMode(Window::m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
    for(;;)
    {
        if (Window::shouldClose()) {
            //tickThread.join();
            break;
        }
        glfwPollEvents();

        cam.updateMovement();
//        cam.updateView();

        Gra::drawFrame(cam);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

// når man holder ned wasd så endrer rotation seg til 2,2,2 men ellers 0,0,0
//        std::cout << "rx: " << cam.rotation.x << " ry: " << cam.rotation.y << " rz: " << cam.rotation.z << std::endl;
    }

    Gra::cleanup();
    Window::destroyWindow();
    return 0;
}
