#include "mat.h"

constexpr void Math::Mat::identity() {
    for (double & element : elements)
        element = 0;
    elements[0] = 1;
    elements[SIZE+1] = 1;
    elements[2*SIZE+2] = 1;
    elements[3*SIZE+3] = 1;
}

constexpr Math::Mat::Mat() {
    identity();
}
