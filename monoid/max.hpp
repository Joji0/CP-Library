#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MaxMonoid {
        using value_type = T;
        static value_type e() { return std::numeric_limits<T>::min(); }
        static value_type op(value_type a, value_type b) { return std::max(a, b); }
};
