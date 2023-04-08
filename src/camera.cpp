//
// Created by Jens Benz on 24.09.2022.
//

#include "camera.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h> 

namespace Camera {

    const bool flipY = true;
    const float movespd = .0035;

    void Cam::updateView()
    {
        // auto eye = glm::vec3(camera->position->x, camera->position->y, camera->position->z);
        // auto center = glm::vec3(*camera->position + *camera->rotation);
        // auto up = glm::vec3(*camera->upOrientation);
        // auto eye = glm::vec3(2.0f, 2.0f, 2.0f);
        // auto center = glm::vec3(0.0f, 0.0f, 0.0f);
        // auto up = glm::vec3(0.0f, 0.0f, 1.0f);
        // camera->view = std::make_shared<glm::mat4>(glm::lookAt(eye, center, up));


        // auto translationMat = glm::translate(glm::mat4(1.0f), -glm::vec3(position));

        // auto rotXMat = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        // auto rotYMat = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        // auto rotZMat = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        // auto rotMat = rotXMat * rotYMat * rotZMat;

        // view = rotMat * translationMat;

        // const float radius = 10.0f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;
        // view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
//		glm::vec3 translation = *position;
//		// if (flipY) {
//			translation.y *= -1.0f;
//		// }
//		// auto transM = glm::translate(glm::mat4(1.0f), translation);
//        // auto mat = glm::mat4
//        view = std::make_shared<glm::mat4>(glm::lookAt(translation, translation + *rotation, *upOrientation));
//        // 	translation.y *= -1.0f;


        // glm::mat4 rotM = glm::mat4(1.0f);
		// glm::mat4 transM;

		// rotM = glm::rotate(rotM, glm::radians(rotation.x * (flipY ? -1.0f : 1.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
		// rotM = glm::rotate(rotM, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		// rotM = glm::rotate(rotM, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));


        // view = rotM * transM;

		// viewPos = glm::vec4(position, 0.0f) * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
    }

    void Cam::updateProjection()
    { 
//        auto proj = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
//        proj[1][1] *= -1; // GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted.
//        projection = std::make_shared<glm::mat4>(proj);
    }

    /*
    Returns whether one should update the view or not.
    */
    // constexpr bool Cam::updateMovement(float x, float z)
    // {
    //     // Forward and backwards + side to side
    //     // auto moveX = (x * moveLongitudinal) + (z * moveSideways);
    //     // auto moveZ = (z * moveLongitudinal) - (x * moveSideways);
    //     // auto moveY = movePerpendicular;

    //     // if (movePerpendicular != 0) {
    //     //     position.y += moveY;
    //     //     change = true;
    //     // }

    //     return change;
    // }

    void  Cam::updateMovement()
    {
        // float x = glm::sin(glm::radians(rotation.y));
//        // float z = glm::cos(glm::radians(rotation.y));
//        bool change = false;
//        if (moveLongitudinal != 0) {
//            *position += *rotation * moveLongitudinal * movespd;
//            change = true;
//        }
//        if (moveSideways != 0) {
//            auto diff = glm::normalize(glm::cross(*rotation, *upOrientation)) * moveSideways * (10.0f*movespd);
//            *position += diff;
//            change = true;
//        }
//        if (movePerpendicular != 0) {
//            position->y += movePerpendicular * (15.0f*movespd);
//            change = true;
//        }
//        if (change) {
//            updateView();
//            std::cout << "x: " << position->x << " y: " << position->y << " z: " << position->z << std::endl;
//            std::cout << "rx: " << rotation->x << " ry: " << rotation->y << " rz: " << rotation->z << std::endl;

//        }
    }

    void Cam::inputMovement(int keycode, bool pressed) {
        switch (keycode)
        {
        case GLFW_KEY_W:
            if (movementFlags.test(0) != pressed) {
                moveLongitudinal += (pressed ? 1 : -1);
                movementFlags[0] = pressed;
            }
            break;
        case GLFW_KEY_A:
            if (movementFlags.test(1) != pressed) {
                moveSideways += (pressed ? -1 : 1);
                movementFlags[1] = pressed;
            }
            break;
        case GLFW_KEY_S:
            if (movementFlags.test(2) != pressed) {
                moveLongitudinal += (pressed ? -1 : 1);
                movementFlags[2] = pressed;
            }
            break;
        case GLFW_KEY_D:
            if (movementFlags.test(3) != pressed) {
                moveSideways += (pressed ? 1 : -1);
                movementFlags[3] = pressed;
            }
            break;
        case GLFW_KEY_SPACE:
            if (movementFlags.test(4) != pressed) {
                movePerpendicular += (pressed ? 1 : -1);
                movementFlags[4] = pressed;
            }
            break;
        case GLFW_KEY_LEFT_SHIFT:
            if (movementFlags.test(5) != pressed) {
                movePerpendicular += (pressed ? -1 : 1);
                movementFlags[5] = pressed;
            }
            break;
        
        default:
            break;
        }
    }

    void Cam::inputMouse(float x, float y)
    {
//        float dx = (x - lastMouseX) * mouseSensitivity;
//        float dy = (y - lastMouseY) * mouseSensitivity;
//
//		lastMouseX = x;
//		lastMouseY = y;
//
//		rotation->x = fmod(rotation->x-dy, 360.0f);
//		rotation->y = fmod(rotation->y+dx, 360.0f);
//        updateView();
//
//        std::cout << "x: " << rotation->x << " y: " << rotation->y << " z: " << rotation->z << std::endl;
		// System.out.println("rot: " + rotation.toString());

        // if (firstMouse)
        // {
        //     lastX = xpos;
        //     lastY = ypos;
        //     firstMouse = false;
        // }
    
        // float xoffset = x - lastMouseX;
        // float yoffset = lastMouseY - y; 
        // lastMouseX = x;
        // lastMouseY = y;

        // float sensitivity = 0.1f;
        // xoffset *= sensitivity;
        // yoffset *= sensitivity;

        // yaw   += xoffset;
        // pitch += yoffset;

        // if(pitch > 89.0f)
        //     pitch = 89.0f;
        // if(pitch < -89.0f)
        //     pitch = -89.0f;

        // glm::vec3 direction;
        // direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        // direction.y = sin(glm::radians(pitch));
        // direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        // rotation = glm::normalize(direction);
    }

} // Camera