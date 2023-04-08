#pragma once

#include <memory>
#include <bitset>
#include "math/vec3.h"
#include "math/mat.h"

namespace Camera
{

    struct Cam {

        std::bitset<6> movementFlags{}; // for remembering movement between key presses
        float fov{ 0.0f };
        float aspect{ 0.0f };
        float nearPlane{ 0.0f };
        float farPlane{ 0.0f };
        float moveLongitudinal{ 0.0f };
        float moveSideways{ 0.0f };
        float movePerpendicular{ 0.0f };

        float lastMouseX{0.0};
		float lastMouseY {0.0};
        float mouseSensitivity{.25f};

        float yaw{ 0.0f };
        float pitch{ 0.0f };

        std::shared_ptr<Math::Vec3> position;
        std::shared_ptr<Math::Vec3> rotation;
        std::shared_ptr<Math::Vec3> upOrientation;
        // oppdateres samtidig med position og rotation slik at det ikke gjøres hver frame men mindre enn hver frame.
        std::shared_ptr<Math::Mat> view; // identity matrix
        std::shared_ptr<Math::Mat> projection; // identity matrix
        
        void updateView();
        void updateProjection();
        // constexpr bool updateMovement(float x, float z);
        void updateMovement();
        void inputMovement(int keycode, bool pressed);
        void inputMouse(float x, float y);
    };

    // void updateView(std::shared_ptr<Cam>& camera);
} // Camera