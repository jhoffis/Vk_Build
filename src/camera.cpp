//
// Created by Jens Benz on 24.09.2022.
//

#include "camera.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h> 

namespace Camera {

    void  Cam::updateMovement()
    {
    }

    void Cam::inputMovement(int keycode, bool pressed) {
        switch (keycode)
        {
        case GLFW_KEY_UP:
            if (movementFlags.test(0) != pressed) {
                moveUpwards += (pressed ? 1 : -1);
                movementFlags[0] = pressed;
            }
            break;
        case GLFW_KEY_DOWN:
            if (movementFlags.test(1) != pressed) {
                moveUpwards += (pressed ? -1 : 1);
                movementFlags[1] = pressed;
            }
            break;
        case GLFW_KEY_LEFT:
            if (movementFlags.test(2) != pressed) {
                moveSideways += (pressed ? -1 : 1);
                movementFlags[2] = pressed;
            }
            break;
        case GLFW_KEY_RIGHT:
            if (movementFlags.test(3) != pressed) {
                moveSideways += (pressed ? 1 : -1);
                movementFlags[3] = pressed;
            }
            break;
        
        default:
            break;
        }
    }

} // Camera
