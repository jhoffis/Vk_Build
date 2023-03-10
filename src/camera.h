#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace Camera
{

    struct Cam {
        bool fwd = false, 
             bck = false, 
             lft = false, 
             rgt = false, 
             up = false, 
             dwn = false; // for remembering movement between key presses
        float fov, aspect, nearPlane, farPlane,
              moveLongitudinal = 0, moveSideways = 0, movePerpendicular = 0;

        std::shared_ptr<glm::vec3> position, rotation, upOrientation;

        // oppdateres samtidig med position og rotation slik at det ikke gjøres hver frame men mindre enn hver frame.
        std::shared_ptr<glm::mat4> view, projection;
    };

    // void updateView(std::shared_ptr<Cam>& camera);
    void updateView(std::shared_ptr<Cam> &camera);
    void updateProjection(std::shared_ptr<Cam> &camera);
    void updateMovement(std::shared_ptr<Cam> &camera);
    void inputMovement(std::shared_ptr<Cam> &camera, int keycode, bool pressed);
} // Camera