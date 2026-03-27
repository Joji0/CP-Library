#pragma once
#include <cstdint>

template <typename T = int64_t> struct MulMonoid {
        using value_type = T;
        static value_type e() { return 1; }
        static value_type op(value_type a, value_type b) { return a * b; }
};
