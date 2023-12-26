#pragma once

#include <vector>
#include "math/Vec2.h"

namespace SelectionBox {

    struct SelectionBoxUBO {
        alignas(8) Vec2 posOriginal;
        alignas(8) Vec2 posNew;
        alignas(8) Vec2 posCam;
        alignas(8) Vec2 resolution;
        alignas(4) float aspect{};
    };

    extern SelectionBoxUBO m_ubo;
    extern std::vector<int> m_selected;

    void init();

    void visible(float x, float y);
    void hide();
    bool isVisible();

    void destroy();

}
