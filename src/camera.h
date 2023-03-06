#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Camera {

    struct Cam {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 upOrientation;

        float fov, aspect, near, far;
        float moveLongitudinal, moveSideways, movePerpendicular;
        bool fwd, bck, lft, rgt, up, dwn; // for remembering movement between key presses

        glm::mat4 view; // oppdateres samtidig med position og rotation slik at det ikke gjøres hver frame men mindre enn hver frame.
        glm::mat4 projection;
    };

    void updateView(Cam* camera);
    void updateProjection(Cam* camera);
    void updateMovement(Cam *camera);
} // Camera