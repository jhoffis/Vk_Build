#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

#include <iostream>
#include "src/vk/gra_setup.h"
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
