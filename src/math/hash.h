#pragma once

#include <cstddef>

namespace Math {

    constexpr void hashCombine(size_t &seed, auto originalValue)
    {
        auto hash = static_cast<size_t>(originalValue);
        hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash;
    }

}