#include "random_util.h"
#include <random>

namespace RandomUtil {
    uint32_t generateNumber() {
        std::random_device r;

        std::default_random_engine e1(r());
        std::uniform_int_distribution<uint32_t> uniform_dist(0, UINT_MAX);

        return uniform_dist(e1);
    }
}
