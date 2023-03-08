#pragma once

#include "window.h"
#include "camera.h"

namespace Gra {

    extern uint32_t currentFrame;

    void createSyncObjects();
    void cleanupSyncObjects();
    void drawFrame(std::shared_ptr<Camera::Cam> camera);

}