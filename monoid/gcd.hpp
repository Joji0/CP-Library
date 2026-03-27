#pragma once
#include <cstdint>
#include <numeric>

template <typename T = int64_t> struct GcdMonoid {
        using value_type = T;
        static value_type e() { return 0; }
        static value_type op(value_type a, value_type b) { return std::gcd(a, b); }
};
