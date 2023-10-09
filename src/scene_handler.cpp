#include "scene_handler.h"

#include <vector>
#include <iostream>

int currentScene = 0;

void SceneHandler::create() {
    static size_t x = 0;
    static size_t y = 0;

//    scenes.emplace_back(0); // feiler om scenes er const / constexpr av en eller annen grunn

    glfwSetKeyCallback(Window::m_window, [](auto window, auto key, auto scancode, auto action, auto mods) {
//        keyInput(&scenes[currentScene], key, action);
        std::cout << "tast key: " << key << ", scancode: " << scancode << ", action: " << action << std::endl;
    });

    glfwSetMouseButtonCallback(Window::m_window, [](auto window, auto button, auto action, auto mods) {
//        mouseButtonInput(&scenes[currentScene], button, action, x, y);
        std::cout << "museklikk x: " << x << ", y: " << y << ", knapp: " << button << std::endl;
    });

    glfwSetCursorPosCallback(Window::m_window, [](auto window, auto xpos, auto ypos) {
        x = xpos;
        y = ypos;
        std::cout << "mus x: " << xpos << ", y: " << ypos << std::endl;
//        mousePosInput(&scenes[currentScene], x, y);
    });
}

void SceneHandler::tick() {

}

void SceneHandler::render() {

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