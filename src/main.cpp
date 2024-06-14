//#define RM_DEV
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// Lage et kattespill som er idle in essence MEN som kan fungere som et slukende tradisjonelt spill - F.eks "Mr. Erik Loves Pussy"
#include <iostream>
#include <thread>
#include <chrono>
#include "vk/gra_setup.h"
#include "vk/drawing/gra_drawing.h"
#include "models/Map.h"
#include "models/Villager.h"
#include "timer_util.h"
#include "src/scene/scene_handler.h"
#include "models/SelectionBoxModel.h"
#include "file_util.h"
#include "rendering/Model.h"
#include "file_watcher.h"
#include "shaders/Shaders.h"

#if RMDEV
#include <cstring>
#include "test/test_selection.h"
int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        Test::run();
        return 0;
    }
#else
int main() {
#endif
//    std::srand(Timer::nowMillis());

    Window::createWindow(false, false);
    Gra::initVulkan();
    Shaders::init();

    SceneHandler::create();
    SelectionBox::init();
    Timer::updateDelta();
#ifdef RMDEV
    watchDir();
#endif


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
//#if RMDEV
//        std::this_thread::sleep_for(std::chrono::milliseconds(2));
//#endif
    }
    vkDeviceWaitIdle(Gra::m_device);

#if RMDEV
    unwatchDir();
#endif

    destroyModels();
    Gra::cleanup();
    Window::destroyWindow();
    return 0;
}
