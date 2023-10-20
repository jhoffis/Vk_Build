#pragma once
#include <bitset>

namespace Camera {

    struct Cam {
        float x{}, y{};
        std::bitset<4> movement;

        void update();
        constexpr void up(bool move) {
            movement.set(0, move);
        }
        constexpr void down(bool move) {
            movement.set(1, move);
        }
        constexpr void left(bool move){
            movement.set(2, move);
        }
        constexpr void right(bool move){
            movement.set(3, move);
        }
    };

    extern Cam m_cam;
} // Camera