#pragma once


#include <vector>

namespace Map {

    struct Map {
        const int xy;
        std::vector<int> map{};
    };

    void createVisual(int xy);
    void destroy();
}
