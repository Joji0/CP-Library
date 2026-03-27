#pragma once
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct SumAssign {
        struct S {
                T val;
                int cnt;
        };
        using F = T;
        static constexpr F NONE = std::numeric_limits<T>::min();
        static S op(S a, S b) { return {a.val + b.val, a.cnt + b.cnt}; }
        static S e() { return {0, 0}; }
        static S mapping(F f, S x) { return f == NONE ? x : S{f * x.cnt, x.cnt}; }
        static F composition(F f, F g) { return f == NONE ? g : f; }
        static F id() { return NONE; }
};
