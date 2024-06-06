#pragma once
#include <cstdint>
#include <cstddef>

namespace MyMath {

    template <typename To, typename From>
    constexpr To bit_cast(const From& from) {
        static_assert(sizeof(To) == sizeof(From), "Size of To and From must be equal");
        union {
            From from;
            To to;
        } u = { from };
        return u.to;
    }

    constexpr float Q_rsprt(float number) {
        const float threehalfs = 1.5F;
        float x2 = number * 0.5F;
        float y = number;

        auto i = bit_cast<uint32_t>(y);
        i = 0x5f3759df - (i >> 1);
        y = bit_cast<float>(i);
        y = y * (threehalfs - (x2 * y * y));
        return y;
    }

    constexpr float fast_sqrt(float number) {
        return 1.0f / Q_rsprt(number);
    }

    constexpr auto max(auto a, auto b) {
        return a > b ? a : b;
    }

}
