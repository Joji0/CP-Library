#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MinMonoid {
        using value_type = T;
        static value_type e() { return std::numeric_limits<T>::max(); }
        static value_type op(value_type a, value_type b) { return std::min(a, b); }
};
