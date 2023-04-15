#pragma once

#include <string>

namespace Math {
    struct Mat {
        static constexpr int SIZE = 4;
        double elements[SIZE*SIZE]{};

        constexpr void operator+=(const Mat& other) {
            for (auto i = 0; i < SIZE * SIZE; i++)
                elements[i] += other.elements[i];
        }

        constexpr void operator*=(const double &scalar) {
            for (double & element : elements)
                element *= scalar;
        }

        constexpr void set(int x, int y, double value) {
            elements[y*SIZE + x] = value;
        }

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