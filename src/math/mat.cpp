#include "mat.h"

namespace Math {
    std::string Mat::toString() {
        std::string str = "[";
        for (auto i = 0; i < SIZE * SIZE; i++) {
            if (i != 0) {
                str.append(", ");
                if (i % 4 == 0)
                    str.append("\n");
            }
            str.append(std::to_string(elements[i]));
        }
        return str.append("]");
    }
}
