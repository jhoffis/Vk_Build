#include <iostream>
#include "src/vk/setup/gra_setup.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Window::createWindow(false, false);
    Gra::initVulkan();

    static bool running = true;
    while(running)
    {
        if (glfwWindowShouldClose(Window::getWindow())) {
            running = false;
            //tickThread.join();
            break;
        }
        glfwPollEvents();

    }

    Gra::cleanup();
    Window::destroyWindow();
    return 0;
}
