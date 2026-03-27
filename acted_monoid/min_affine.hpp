#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MinAffine {
        using S = T;
        struct F {
                T a, b;
        };
        static S op(S a, S b) { return std::min(a, b); }
        static S e() { return std::numeric_limits<T>::max(); }
        static S mapping(F f, S x) { return f.a * x + f.b; }
        static F composition(F f, F g) { return {f.a * g.a, f.a * g.b + f.b}; }
        static F id() { return {1, 0}; }
};
