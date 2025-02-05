//
// Created by Jens Benz on 24.09.2022.
//

#include "camera.h"
#include "timer_util.h"

namespace Camera {
    Cam m_cam{};

    void Cam::update() {
        auto spd = static_cast<float>(1.75 * Timer::delta());
        if (movement.test(0))
            pos.y += spd;
        if (movement.test(1))
            pos.y -= spd;
        if (movement.test(2))
            pos.x -= spd;
        if (movement.test(3))
            pos.x += spd;
    }
} // Camera