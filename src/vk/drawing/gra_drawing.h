#pragma once

#include "window.h"
#include "camera.h"
#include "vk/shading/model.h"
#include "vk/shading/shader.h"

namespace Gra {

    extern uint32_t currentFrame;

    void createSyncObjects();
    void cleanupSyncObjects();
    void drawFrame(Camera::Cam &camera, Model::Mesh meshes[], Shader::ShaderData &shader);

}