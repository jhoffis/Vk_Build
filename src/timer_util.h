#include <cstdint>

namespace Timer {
    uint64_t nowMillis();
    uint64_t nowNanos();
    void updateDelta();
    double delta();
};