#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <bitset>

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

        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
        glm::vec3 upOrientation{ 0.0f, 0.0f, 0.0f };
        // oppdateres samtidig med position og rotation slik at det ikke gjøres hver frame men mindre enn hver frame.
        glm::mat4 view{ 1.0f }; // identity matrix
        glm::mat4 projection{ 1.0f }; // identity matrix
        
        void updateView();
        void updateProjection();
        // constexpr bool updateMovement(float x, float z);
        void updateMovement();
        void inputMovement(int keycode, bool pressed);
        void inputMouse(float x, float y);
    };

    // void updateView(std::shared_ptr<Cam>& camera);
} // Camera