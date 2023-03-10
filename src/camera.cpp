//
// Created by Jens Benz on 24.09.2022.
//

#include "camera.h"

#include <GLFW/glfw3.h>
#include <iostream>

namespace Camera {

    void updateView(std::shared_ptr<Cam>& camera)
    {
        auto eye = glm::vec3(camera->position->x, camera->position->y, camera->position->z);
        auto center = glm::vec3(*camera->position + *camera->rotation);
        auto up = glm::vec3(*camera->upOrientation);
        // auto eye = glm::vec3(2.0f, 2.0f, 2.0f);
        // auto center = glm::vec3(0.0f, 0.0f, 0.0f);
        // auto up = glm::vec3(0.0f, 0.0f, 1.0f);
        camera->view = std::make_shared<glm::mat4>(glm::lookAt(eye, center, up));
    }

    void updateProjection(std::shared_ptr<Cam>& camera)
    { 
        float fov = camera->fov;
        float aspect = camera->aspect;
        float nearPlane = camera->nearPlane;
        float farPlane = camera->farPlane;
        auto proj = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
        proj[1][1] *= -1; // GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted.
        camera->projection = std::make_shared<glm::mat4>(proj);
    }

    /*
    Returns whether one should update the view or not.
    */
    bool updateMovement(std::shared_ptr<Cam>& camera, float x, float z)
    {
        // Forward and backwards + side to side
        auto moveX = (x * camera->moveLongitudinal) + (z * camera->moveSideways);
        auto moveZ = (z * camera->moveLongitudinal) - (x * camera->moveSideways);
        auto moveY = camera->movePerpendicular * .01f;

        bool change = false;
        if (moveX != 0) {
            camera->position->x += moveX;
            change = true;
        }
        if (moveZ != 0) {
            camera->position->z += moveZ;
            change = true;
        }
        if (moveY != 0) {
            camera->position->y += moveY;
            change = true;
        }

        return change;
    }

    void updateMovement(std::shared_ptr<Cam>& camera)
    {
        float x = glm::sin(glm::radians(camera->rotation->y)) * .01f;
        float z = glm::cos(glm::radians(camera->rotation->y)) * .01f;
        if (updateMovement(camera, x, z)) {
            updateView(camera);
            std::cout << "x: " << camera->position->x << "z: " << camera->position->z << std::endl;
        }
    }

    void inputMovementReal(std::shared_ptr<Cam>& camera, int keycode, bool pressed) {
        switch (keycode)
        {
        case GLFW_KEY_W:
            if (camera->fwd != pressed)
                camera->moveLongitudinal += pressed ? 1 : -1;
            camera->fwd = pressed;
            break;
        case GLFW_KEY_A:
            if (camera->lft != pressed)
                camera->moveSideways += pressed ? 1 : -1;
            camera->lft = pressed;
            break;
        case GLFW_KEY_S:
            if (camera->bck != pressed)
                camera->moveLongitudinal += pressed ? -1 : 1;
            camera->bck = pressed;
            break;
        case GLFW_KEY_D:
            if (camera->rgt != pressed)
                camera->moveSideways += pressed ? -1 : 1;
            camera->rgt = pressed;
            break;
        case GLFW_KEY_SPACE:
            if (camera->up != pressed)
                camera->movePerpendicular += pressed ? 1 : -1;
            camera->up = pressed;
            break;
        case GLFW_KEY_LEFT_SHIFT:
            if (camera->dwn != pressed)
                camera->movePerpendicular += pressed ? -1 : 1;
            camera->dwn = pressed;
            break;
        
        default:
            break;
        }
    }
    void inputMovement(std::shared_ptr<Cam>& camera, int keycode, bool pressed) {
        inputMovementReal(camera, keycode, pressed);
    }

} // Camera