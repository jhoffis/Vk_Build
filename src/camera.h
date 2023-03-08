#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace Camera {

    struct Cam {
        bool fwd, bck, lft, rgt, up, dwn; // for remembering movement between key presses
        float fov, aspect, nearPlane, farPlane;
        float moveLongitudinal, moveSideways, movePerpendicular;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 upOrientation;

        glm::mat4 view; // oppdateres samtidig med position og rotation slik at det ikke gjøres hver frame men mindre enn hver frame.
        glm::mat4 projection;
    };

    // void updateView(std::shared_ptr<Cam>& camera);
    void updateView(Cam* camera);
    void updateProjection(std::shared_ptr<Cam>& camera);
    void updateMovement(std::shared_ptr<Cam>& camera);
    void inputMovement(std::shared_ptr<Cam>& camera, int keycode, bool pressed);
} // Camera