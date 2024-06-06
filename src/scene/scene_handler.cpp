#include "scene_handler.h"
#include "models/Map.h"
#include "models/Villager.h"
#include "camera.h"
#include "timer_util.h"
#include "models/SelectionBoxModel.h"
#include "scene_data.h"
#include "rendering/Model.h"
#include "path_finding.h"

#include <vector>
#include <iostream>

static size_t x = 0;
static size_t y = 0;

void SceneHandler::create() {

    Map::createMap(15);
    Map::m_map->map[17] = 0;
    Map::m_map->map[18] = 0;
    Map::m_map->map[19] = 0;
    Map::m_map->map[47] = 0;
    Map::m_map->map[48] = 0;
    Map::m_map->map[49] = 0;

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
                    SceneData::xWorldDragCam = SceneData::xWorld;
                    SceneData::yWorldDragCam = SceneData::yWorld;
                    SceneData::dragging = true;
                } else if (action == GLFW_RELEASE) {
                    SceneData::dragging = false;
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

                SelectionBox::visible(SceneData::xWorld, SceneData::yWorld);

            } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
                SceneData::xWorldDragCam = SceneData::xWorld;
                SceneData::yWorldDragCam = SceneData::yWorld;
                SceneData::dragging = true;
            } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {


                for (auto vill : Villager::m_selectedVills) {
                    std::vector<int> OutPath;
                    auto start = Map::worldToMapCoordinates(vill->entity->pos.x, vill->entity->pos.y);
                    auto target = Map::worldToMapCoordinates(SceneData::xWorld, SceneData::yWorld);
                    auto res3 = PathFinder::findPath(
                            start,
                            target,
                            *Map::m_map,
                            OutPath
                    );
                    if (res3) {
                        vill->pathIndex = 0;
                        PathFinder::convertMapPathToWorldPath(*Map::m_map, OutPath, (std::vector<Vec2> &) vill->path);
                    }


//                    vill->entity->pos.x = SceneData::xWorld;
//                    vill->entity->pos.y = SceneData::yWorld;
                }
            }

        } else {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                SelectionBox::hide(true);
            } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
                SceneData::dragging = false;
            }
        }
    });

    glfwSetCursorPosCallback(Window::m_window, [](auto window, auto xPos, auto yPos) {
        x = xPos;
        y = yPos;

        // world er basert på høyde av skjermen!
        auto h = static_cast<double>(Window::HEIGHT);
        SceneData::xWorld = (xPos / h) + Camera::m_cam.pos.x;
        SceneData::yWorld = 1. - (yPos / h) + Camera::m_cam.pos.y;

        if (SceneData::dragging) {
            Camera::m_cam.pos.x += SceneData::xWorldDragCam - SceneData::xWorld;
            Camera::m_cam.pos.y += SceneData::yWorldDragCam - SceneData::yWorld;
        }

        if (SelectionBox::isVisible()) {
            SelectionBox::m_ubo.posNew.x = SceneData::xWorld;
            SelectionBox::m_ubo.posNew.y = SceneData::yWorld;
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


    Map::createVisual(Map::m_map->xy);
    Villager::initVillModel();
    Villager::spawn(-2, 3);
    Villager::spawn(1.75f, 1.33);
    Villager::spawn(1.5f, 1);
    Villager::spawn(2, 1);
    Villager::spawn(1.7f, .75);
    Villager::spawn(0, 0);
    Villager::spawn(0, 0);
}

void SceneHandler::update() {
    Camera::m_cam.update();
    Shaders::m_villModel.sort();
    Villager::sort();
    Villager::update();
//    if (selected != nullptr && !selected->entity.isAbove(xWorld, yWorld))
//        selected->entity.pos.x += static_cast<float>(.1 * Timer::delta());
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