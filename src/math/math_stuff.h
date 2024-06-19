#pragma once
#include <cstdint>

namespace MyMath {

template <typename To, typename From> constexpr To bit_cast(const From &from) {
  static_assert(sizeof(To) == sizeof(From),
                "Size of To and From must be equal");
  union {
    From from;
    To to;
  } u = {from};
  return u.to;
}

constexpr float Q_rsprt(float number) {
  const float threehalfs = 1.5F;
  float x2 = number * 0.5F;
  float y = number;

  auto i = bit_cast<uint32_t>(y);
  i = 0x5f3759df - (i >> 1);
  y = bit_cast<float>(i);
  y = y * (threehalfs - (x2 * y * y));
  return y;
}

constexpr float fast_sqrt(float number) { return 1.0f / Q_rsprt(number); }

constexpr int abs(int x) {
  auto y = x >> 31;
  return (x ^ y) - y;
}

constexpr auto min(auto a, auto b) { return a < b ? a : b; }

constexpr auto max(auto a, auto b) { return a > b ? a : b; }

constexpr int nextPowerOfTwo(int n) {
  n = (n <= 0) ? 1 : n;

  // Decrement n (to ensure we get the next power of 2 if n is already a power
  // of 2)
  n -= 1;

  // Set all bits after the highest set bit
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;

  // Increment n to get the next power of 2
  return 2 * (n + 1);
}

} // namespace MyMath
