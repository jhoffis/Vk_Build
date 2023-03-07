//
// Created by Jens Benz on 24.09.2022.
//

#include "camera.h"

#include <GLFW/glfw3.h>


namespace Camera {

    void updateView(Cam *camera)
    {
        camera->view = glm::lookAt(camera->position, camera->position + camera->rotation, camera->upOrientation);
    }

    void updateProjection(Cam *camera)
    {
        camera->projection = glm::perspective(glm::radians(camera->fov), camera->aspect, camera->near, camera->far);
    }

    /*
    Returns whether one should update the view or not.
    */
    constexpr bool updateMovement(Cam *camera, float x, float z)
    {
        // Forward and backwards + side to side
        auto moveX = (x * camera->moveLongitudinal) + (z * camera->moveSideways);
        auto moveZ = (z * camera->moveLongitudinal) - (x * camera->moveSideways);
        auto moveY = camera->movePerpendicular;

        bool change = false;
        if (moveX != 0) {
            camera->position.x += moveX;
            change = true;
        }
        if (moveZ != 0) {
            camera->position.z += moveZ;
            change = true;
        }
        if (moveY != 0) {
            camera->position.y += moveY;
            change = true;
        }

        return change;
    }

    void updateMovement(Cam *camera)
    {
        float x = glm::sin(glm::radians(camera->rotation.y)) * 1;
        float z = glm::cos(glm::radians(camera->rotation.y)) * 1;
        if (updateMovement(camera, x, z))
            updateView(camera);
    }

    constexpr void inputMovement(Cam *camera, int keycode, bool pressed) {
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

} // Camera