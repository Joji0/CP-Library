#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MinAdd {
        using S = T;
        using F = T;
        static S op(S a, S b) { return std::min(a, b); }
        static S e() { return std::numeric_limits<T>::max(); }
        static S mapping(F f, S x) { return x + f; }
        static F composition(F f, F g) { return f + g; }
        static F id() { return 0; }
};
