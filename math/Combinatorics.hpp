#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>
#include <limits>
#include "math/ModInt.hpp"

template <typename T>
class Combinatorics {
        std::vector<T> f, inv_f, inv_n;
        void extend(int m) {
                int cur = (int)f.size();
                if (cur > m) return;
                int next_sz = std::max(m + 1, cur * 2);
                f.resize(next_sz);
                inv_f.resize(next_sz);
                inv_n.resize(next_sz);
                for (int i = cur; i < next_sz; ++i) {
                        f[i] = f[i - 1] * T(i);
                }
                inv_f[next_sz - 1] = f[next_sz - 1].inv();
                for (int i = next_sz - 2; i >= cur; --i) {
                        inv_f[i] = inv_f[i + 1] * T(i + 1);
                }
                for (int i = cur; i < next_sz; ++i) {
                        inv_n[i] = inv_f[i] * f[i - 1];
                }
        }
public:
        Combinatorics(int max_n = 2) {
                int sz = std::max(2, max_n + 1);
                f.resize(sz);
                inv_f.resize(sz);
                inv_n.resize(sz);
                f[0] = f[1] = T(1);
                inv_f[0] = inv_f[1] = T(1);
                inv_n[1] = T(1);
                for (int i = 2; i < sz; ++i) {
                        f[i] = f[i - 1] * T(i);
                }
                inv_f[sz - 1] = f[sz - 1].inv();
                for (int i = sz - 2; i >= 1; --i) {
                        inv_f[i] = inv_f[i + 1] * T(i + 1);
                }
                for (int i = 2; i < sz; ++i) {
                        inv_n[i] = inv_f[i] * f[i - 1];
                }
        }
        T fact(int n) {
                if (n < 0) return T(0);
                if (n >= (int)f.size()) extend(n);
                return f[n];
        }
        T inv_fact(int n) {
                if (n < 0) return T(0);
                if (n >= (int)inv_f.size()) extend(n);
                return inv_f[n];
        }
        T inv(int n) {
                assert(n > 0);
                if (n >= (int)inv_n.size()) extend(n);
                return inv_n[n];
        }
        T nCr(int64_t n, int64_t r) {
                if (r < 0 || r > n) return T(0);
                if (n < (int)f.size()) return f[n] * inv_f[r] * inv_f[n - r];
                if (n < (1 << 22)) {
                        extend((int)n);
                        return f[n] * inv_f[r] * inv_f[n - r];
                }
                return nCr_large(n, r);
        }
        T nPr(int64_t n, int64_t r) {
                if (r < 0 || r > n) return T(0);
                if (n >= (int)f.size()) extend((int)n);
                return f[n] * inv_f[n - r];
        }
        T nHr(int64_t n, int64_t r) {
                if (n < 0 || r < 0) return T(0);
                if (n == 0 && r == 0) return T(1);
                if (n == 0) return T(0);
                return nCr(n + r - 1, r);
        }
        T nCr_large(int64_t n, int64_t r) {
                if (r < 0 || r > n) return T(0);
                if (r > n - r) r = n - r;
                if (r == 0) return T(1);
                if (r >= (int)inv_f.size()) extend((int)r);
                T num = 1;
                for (int64_t i = 0; i < r; ++i) {
                        num *= T(n - i);
                }
                return num * inv_fact((int)r);
        }
        T catalan(int n) {
                if (n < 0) return T(0);
                return nCr(2 * n, n) * inv(n + 1);
        }
        T derangement(int n) {
                if (n < 0) return T(0);
                if (n == 0) return T(1);
                if (n == 1) return T(0);
                T ans = 0;
                for (int i = 0; i <= n; ++i) {
                        T term = inv_fact(i);
                        if (i % 2 == 1) ans -= term;
                        else ans += term;
                }
                return ans * fact(n);
        }
};

namespace NonModCombinatorics {

template <typename T = int64_t>
inline T nCr(int64_t n, int64_t r) {
        if (r < 0 || r > n) return T(0);
        if (r > n - r) r = n - r;
        if (r == 0) return T(1);
        T res = 1;
        for (int64_t i = 1; i <= r; ++i) {
                res = res * (n - r + i) / i;
        }
        return res;
}

template <typename T = int64_t>
inline std::vector<std::vector<T>> pascal_triangle(int n) {
        std::vector<std::vector<T>> c(n + 1, std::vector<T>(n + 1, T(0)));
        for (int i = 0; i <= n; ++i) {
                c[i][0] = T(1);
                for (int j = 1; j <= i; ++j) {
                        c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
                }
        }
        return c;
}

} // namespace NonModCombinatorics
