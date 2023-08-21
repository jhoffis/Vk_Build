#include "timer_util.h"
#include <chrono>
using namespace std::chrono;

long lastLoopTime;

uint64_t Timer::nowMillis() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
//
//double Timer::nowDelta() {
//    auto time = nowMillis();
//    double delta = time - lastLoopTime;
//    lastLoopTime = time;
//    return delta / 40000000.0;
//}