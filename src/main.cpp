#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


// Lage et kattespill som er idle in essence MEN som kan fungere som et slukende tradisjonelt spill - F.eks "Mr. Erik Loves Pussy"

#include <iostream>
#include "vk/gra_setup.h"
#include "vk/drawing/gra_drawing.h"
#include "game/Map.h"
#include "game/Villager.h"
#include "network/RemoteLan.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Window::createWindow(false, false);
    Gra::initVulkan();

    Map::create(30);
    Villager::createVillagers();

    static bool running = true;
    while(running)
    {
        if (glfwWindowShouldClose(Window::m_window)) {
            running = false;
            //tickThread.join();
            break;
        }
        glfwPollEvents();
        Drawing::drawFrame();
    }

    vkDeviceWaitIdle(Gra::m_device);

    Map::destroy();
    Villager::destroy();
    Gra::cleanup();
    Window::destroyWindow();
    return 0;
}
