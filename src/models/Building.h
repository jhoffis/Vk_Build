#pragma once

namespace Building {
    void startHovering(float wX, float wY);
    void stopHovering();
    bool isHovering();
    void updateHovering(float wX, float wY);
    void place(float wX, float wY);
}
