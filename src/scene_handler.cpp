#include "scene_handler.h"
#include "game/Map.h"
#include "game/Villagers.h"
#include "camera.h"
#include "timer_util.h"
#include "gra_elems/fabrications/SelectionBoxModel.h"

#include <vector>
#include <iostream>

int currentScene = 0;
std::vector<Villager::Vill *> males{};
Villager::Vill *selected = nullptr;
static size_t x = 0;
static size_t y = 0;
static double xWorld{};
static double yWorld{};
double xWorldDragCam{};
double yWorldDragCam{};
bool dragging{};

void SceneHandler::create() {

    glfwSetKeyCallback(Window::m_window, [](auto window, auto key, auto scancode, auto action, auto mods) {
//        keyInput(&scenes[currentScene], key, action);
        std::cout << "tast key: " << key << ", scancode: " << scancode << ", action: " << action << std::endl;

        switch (key) {
            case GLFW_KEY_UP:
                Camera::m_cam.up(action != GLFW_RELEASE);
                break;
            case GLFW_KEY_DOWN:
                Camera::m_cam.down(action != GLFW_RELEASE);
                break;
            case GLFW_KEY_LEFT:
                Camera::m_cam.left(action != GLFW_RELEASE);
                break;
            case GLFW_KEY_RIGHT:
                Camera::m_cam.right(action != GLFW_RELEASE);
                break;
            case GLFW_KEY_SPACE:
                if (action == GLFW_PRESS) {
                    xWorldDragCam = xWorld;
                    yWorldDragCam = yWorld;
                    dragging = true;
                } else if (action == GLFW_RELEASE) {
                    dragging = false;
                }
                break;
            default:
                // do nothing
                break;
        }
        std::cout << "camera world x: " << Camera::m_cam.pos.x << ", y: " << Camera::m_cam.pos.y << std::endl;
    });

    glfwSetMouseButtonCallback(Window::m_window, [](auto window, auto button, auto action, auto mods) {
//        mouseButtonInput(&scenes[currentScene], button, action, x, y);
        std::cout << "museklikk x: " << x << ", y: " << y << ", knapp: " << button << std::endl;

        if (action != GLFW_RELEASE) {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {

                SelectionBox::visible(xWorld, yWorld);

                bool found = false;
                for (auto vill: males) {
                    if (vill->entity.isAbove(xWorld, yWorld)) {
                        selected = vill;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    selected = nullptr;
            } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
                xWorldDragCam = xWorld;
                yWorldDragCam = yWorld;
                dragging = true;
            }
        } else {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                SelectionBox::hide();
            } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
                dragging = false;
            }
        }
    });

    glfwSetCursorPosCallback(Window::m_window, [](auto window, auto xPos, auto yPos) {
        x = xPos;
        y = yPos;

        // world er basert på høyde av skjermen!
        auto h = static_cast<double>(Window::HEIGHT);
        xWorld = (xPos / h) + Camera::m_cam.pos.x;
        yWorld = 1. - (yPos / h) + Camera::m_cam.pos.y;

        if (dragging) {
            Camera::m_cam.pos.x += xWorldDragCam - xWorld;
            Camera::m_cam.pos.y += yWorldDragCam - yWorld;
        }

        if (SelectionBox::isVisible()) {
            SelectionBox::m_ubo.posNew.x = xWorld;
            SelectionBox::m_ubo.posNew.y = yWorld;
        }

//        std::cout << "mus x: " << xPos << ", y: " << yPos
//                  << ", world x: " << xWorld << ", y: " << yWorld << std::endl;
    });

//    glfwSetScrollCallback(Window::m_window, [](auto window, auto xoffset, auto yoffset) {
//        Camera::m_cam.pos.z *= 1+.125*yoffset;
//        if (Camera::m_cam.pos.z < -2.)
//            Camera::m_cam.pos.z = -2.;
//        else if (Camera::m_cam.pos.z > -.5)
//            Camera::m_cam.pos.z = -.5;
//        std::cout << "scroll x: " << xoffset << ", y: " << yoffset << "cam z: " << Camera::m_cam.pos.z << std::endl;
//    });


    Map::create(15);
    Villager::initVillModel();
    males.emplace_back(Villager::spawnMale(0, 0));
    males.emplace_back(Villager::spawnMale(1.5f, 1));
    males.emplace_back(Villager::spawnMale(2, 1));
    males.emplace_back(Villager::spawnMale(2, 3));
}

void SceneHandler::update() {
    Camera::m_cam.update();
    if (selected != nullptr && !selected->entity.isAbove(xWorld, yWorld))
        selected->entity.pos.x += static_cast<float>(.1 * Timer::delta());
//    for (auto male : males)
//        male->entity.pos.x += static_cast<float>(.001 * Timer::delta());
//    std::cout << males[0].vill.entity.pos.x << std::endl;
}


/*
SceneHandler::SceneHandler(std::vector<SceneEnvir> &scenes) : scenes(scenes) {

}

void SceneHandler::updateGenerally() {
    scenes.at(Scenes::CURRENT).updateGenerally();
}


void SceneHandler::updateResolution() {
//    UISceneInfo.updateResolution();

    for (int i = 0; i < sizeof(scenes); i++) {
        scenes[i].updateResolution();
    }
}

void SceneHandler::changeScene(int scenenr, bool logCurrent) {
    if (logCurrent) {
        if (scenenr == Scenes::PREVIOUS)
        {
            do {
                scenenr = Scenes::HISTORY.top();
                Scenes::HISTORY.pop();
            } while (!Scenes::HISTORY.empty() && (Scenes::HISTORY.top() == scenenr || scenenr == Scenes::CURRENT));
        }
        Scenes::HISTORY.push(Scenes::CURRENT);
    }
    Scenes::CURRENT = scenenr;

    // Weird previous ik.
    if (Scenes::CURRENT < Scenes::OPTIONS)
        Scenes::PREVIOUS_REGULAR = Scenes::CURRENT;

}


void SceneHandler::renderUILayout(NkContext ctx) {
}

void SceneHandler::keyInput(int keycode, int action) {
    std::cout << keycode <<  std::endl;
}

void SceneHandler::mouseButtonInput(int button, int action, double x, double y) {
    std::cout << action << ' ' << x << ' ' << y << std::endl;
}

void SceneHandler::mousePosInput(double x, double y) {
    std::cout << x << ' ' << y << std::endl;
}

void SceneHandler::mouseScrollInput(double x, double y) {
    std::cout << y << std::endl;
}
*/