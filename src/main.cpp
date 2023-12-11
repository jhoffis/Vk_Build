#define RM_DEV
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


// Lage et kattespill som er idle in essence MEN som kan fungere som et slukende tradisjonelt spill - F.eks "Mr. Erik Loves Pussy"

#include <iostream>
#include <thread>
#include <chrono>
#include "vk/gra_setup.h"
#include "vk/drawing/gra_drawing.h"
#include "game/Map.h"
#include "game/Villagers.h"
#include "timer_util.h"
#include "scene_handler.h"
#include "src/gra_elems/fabrications/SelectionBoxModel.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    std::srand(Timer::nowMillis());

    Window::createWindow(false, false);
    Gra::initVulkan();
    SceneHandler::create();
    Timer::updateDelta();
    SelectionBox::init();

//    Model testModel{};
//    testModel.init("triangle", "grass.png");
//
//    auto entity2 = testModel.addEntity(false);
//    entity2->pos.x = testModel.width() * 1;
//    entity2->pos.y = testModel.height() * 1;
//
//    auto entity = testModel.addEntity(false);
//    entity->pos.x = testModel.width() * 0;
//    entity->pos.y = testModel.height() * 1;
//
//    testModel.updateUboBuffer();
//    m_renderModels.emplace_back(&testModel);

    int fps = 0;
    double fpsTime = 0;

    static bool running = true;
    while(running)
    {
        if (glfwWindowShouldClose(Window::m_window)) {
            running = false;
            //tickThread.join();
            break;
        }
        glfwPollEvents();
        Timer::updateDelta();
        SceneHandler::update();
        Drawing::drawFrame();

        fps++;
        fpsTime += Timer::delta();
        if (fpsTime >= 1.) {
            Window::setName("vk FPS: " + std::to_string(fps));
            fpsTime -= 1.;
            fps = 0;
        }
#ifdef RM_DEV
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
#endif
    }

    vkDeviceWaitIdle(Gra::m_device);

//    testModel.destroy();
    SelectionBox::destroy();
    Map::destroy();
    Villager::destroy();
    Gra::cleanup();
    Window::destroyWindow();
    return 0;
}
