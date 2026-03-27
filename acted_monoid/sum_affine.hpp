#pragma once
#include <cstdint>

template <typename T = int64_t> struct SumAffine {
        struct S {
                T val;
                int cnt;
        };
        struct F {
                T a, b;
        };
        static S op(S x, S y) { return {x.val + y.val, x.cnt + y.cnt}; }
        static S e() { return {0, 0}; }
        static S mapping(F f, S x) { return {f.a * x.val + f.b * x.cnt, x.cnt}; }
        static F composition(F f, F g) { return {f.a * g.a, f.a * g.b + f.b}; }
        static F id() { return {1, 0}; }
};
