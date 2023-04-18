#pragma once

#include <memory>
#include <bitset>
#include "math/vec3.h"
#include "math/mat.h"

namespace Camera
{

    struct Cam {

        std::bitset<6> movementFlags{}; // for remembering movement between key presses
        float moveUpwards{ 0.0f };
        float moveSideways{ 0.0f };

        float lastMouseX{0.0};
	float lastMouseY {0.0};
        float mouseSensitivity{.25f};

        Math::Vec3 position{};
        
        void updateMovement();
        void inputMovement(int keycode, bool pressed);
       // void inputMouse(float x, float y);
    };

    // void updateView(std::shared_ptr<Cam>& camera);
} // Camera
