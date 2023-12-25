#include <cstdint>
#include <chrono>

namespace Timer {
    uint64_t nowMillis();
    uint64_t nowMillisFile();
    uint64_t toMillis(std::chrono::time_point<std::chrono::file_clock> timePoint);

    uint64_t nowNanos();

    void updateDelta();
    double delta();

};