#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MinAssign {
        using S = T;
        using F = T;
        static constexpr F NONE = std::numeric_limits<T>::max();
        static S op(S a, S b) { return std::min(a, b); }
        static S e() { return std::numeric_limits<T>::max(); }
        static S mapping(F f, S x) { return f == NONE ? x : f; }
        static F composition(F f, F g) { return f == NONE ? g : f; }
        static F id() { return NONE; }
};
