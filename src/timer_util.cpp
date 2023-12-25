#include "timer_util.h"
#include <chrono>
using namespace std::chrono;

uint64_t m_lastLoopTime{};
double m_delta{};

uint64_t Timer::nowMillis() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

uint64_t Timer::nowMillisFile() {
    return duration_cast<milliseconds>(file_clock::now().time_since_epoch()).count();
}

uint64_t Timer::toMillis(std::chrono::time_point<std::chrono::file_clock> timePoint) {
    return duration_cast<milliseconds>(timePoint.time_since_epoch()).count();
}

uint64_t Timer::nowNanos() {
    return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

void Timer::updateDelta() {
    auto time = nowNanos();
    auto delta = static_cast<double>(time - m_lastLoopTime);
    m_lastLoopTime = time;
    m_delta = delta / 1000'000'000.0;
}

double Timer::delta() {
    return m_delta;
}
