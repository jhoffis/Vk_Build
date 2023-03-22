#pragma once

namespace Math {
    struct Mat {
        static constexpr int SIZE = 4;
        double elements[SIZE*SIZE]{};

        constexpr void identity();
        constexpr Mat();
    };
}