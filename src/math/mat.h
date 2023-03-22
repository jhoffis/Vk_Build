#pragma once

#include <string>

namespace Math {
    struct Mat {
        static constexpr int SIZE = 4;
        double elements[SIZE*SIZE]{};

        constexpr void identity() {
            for (auto &element: elements)
                element = 0;
            elements[0] = 1;
            elements[SIZE + 1] = 1;
            elements[2 * SIZE + 2] = 1;
            elements[3 * SIZE + 3] = 1;
        }
        constexpr Mat() {
            identity();
        }

        std::string toString();
    };
}