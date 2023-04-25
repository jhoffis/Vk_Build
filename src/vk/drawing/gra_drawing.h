#pragma once

#include "window.h"
#include "camera.h"
#include "vk/shading/model.h"

namespace Gra {

    extern uint32_t currentFrame;

    void createSyncObjects();
    void cleanupSyncObjects();
    void drawFrame(Camera::Cam &camera, Model::Mesh mesh);

}