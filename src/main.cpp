#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


// Lage et kattespill som er idle in essence MEN som kan fungere som et slukende tradisjonelt spill - F.eks "Mr. Erik Loves Pussy"

#include <iostream>
#include "src/vk/gra_setup.h"
#include "src/vk/drawing/gra_drawing.h"
#include "gra/Model.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Window::createWindow(false, false);
    Gra::initVulkan();

    Model testModel{};
    m_renderModels.emplace_back(&testModel);
    Model testModel2{};
    testModel2.x = 1.0f;
    m_renderModels.emplace_back(&testModel2);

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

    testModel2.destroy();
    testModel.destroy();
    Gra::cleanup();
    Window::destroyWindow();
    return 0;
}
