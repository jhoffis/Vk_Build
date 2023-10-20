//
// Created by Jens Benz on 24.09.2022.
//

#include "camera.h"
#include "timer_util.h"

namespace Camera {
    Cam m_cam{};

    void Cam::update() {
        auto spd = static_cast<float>(.075 * Timer::delta());
        if (movement.test(0))
            y += spd;
        if (movement.test(1))
            y -= spd;
        if (movement.test(2))
            x -= spd;
        if (movement.test(3))
            x += spd;
    }
} // Camera