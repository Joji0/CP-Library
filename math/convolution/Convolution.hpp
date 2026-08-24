#pragma once
#include <vector>
#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <cassert>
#include "math/ModInt.hpp"

namespace Convolution {

constexpr uint64_t primitive_root_for_convolution(uint64_t p) {
        if (p == 2) return 1;
        if (p == 998244353) return 3;
        if (p == 469762049) return 3;
        if (p == 1811939329) return 11;
        if (p == 2013265921) return 11;
        for (uint64_t g = 2; g < p; ++g) {
                uint64_t base = g, exp = (p - 1) >> 1, res = 1;
                while (exp > 0) {
                        if (exp & 1) res = (res * base) % p;
                        base = (base * base) % p;
                        exp >>= 1;
                }
                if (res != 1) return g;
        }
        return -1;
}

namespace internal {

constexpr int msb(uint32_t x) { return x == 0 ? -1 : 31 - __builtin_clz(x); }
constexpr int ceil_log2(uint32_t x) { return x <= 1 ? 0 : 32 - __builtin_clz(x - 1); }

template <class T>
class NthRoot {
private:
        static constexpr unsigned int lg = msb((T::get_mod() - 1) & (1 - T::get_mod()));
        T root[lg + 1];
        T inv_root[lg + 1];
        T rate[lg + 1];
        T inv_rate[lg + 1];

public:
        constexpr NthRoot() : root{}, inv_root{}, rate{}, inv_rate{} {
                root[lg] = T(primitive_root_for_convolution(T::get_mod())).pow((T::get_mod() - 1) >> lg);
                inv_root[lg] = root[lg].inv();
                for (int i = lg - 1; i >= 0; i--) {
                        root[i] = root[i + 1] * root[i + 1];
                        inv_root[i] = inv_root[i + 1] * inv_root[i + 1];
                }
                T r = 1;
                for (int i = 2; i <= (int)lg; i++) {
                        rate[i - 2] = r * root[i];
                        r = r * inv_root[i];
                }
                r = 1;
                for (int i = 2; i <= (int)lg; i++) {
                        inv_rate[i - 2] = r * inv_root[i];
                        r = r * root[i];
                }
        }
        static constexpr unsigned int get_lg() { return lg; }
        constexpr T get(int n) const { return root[n]; }
        constexpr T inv(int n) const { return inv_root[n]; }
        constexpr T get_rate(int n) const { return rate[n]; }
        constexpr T get_inv_rate(int n) const { return inv_rate[n]; }
};

template <class T>
void number_theoretic_transform(std::vector<T>& a) {
        static constexpr NthRoot<T> nth_root;
        int n = (int)a.size();
        for (int i = n >> 1; i > 0; i >>= 1) {
                T z = T::raw(1);
                for (int j = 0; j < n; j += (i << 1)) {
                        for (int k = 0; k < i; k++) {
                                const T x = a[j + k];
                                const T y = a[j + i + k] * z;
                                a[j + k] = x + y;
                                a[j + i + k] = x - y;
                        }
                        z *= nth_root.get_rate(__builtin_popcount(j & ~(j + (i << 1))));
                }
        }
}

template <class T>
void inverse_number_theoretic_transform(std::vector<T>& a) {
        static constexpr NthRoot<T> nth_root;
        int n = (int)a.size();
        for (int i = 1; i < n; i <<= 1) {
                T z = T::raw(1);
                for (int j = 0; j < n; j += (i << 1)) {
                        for (int k = 0; k < i; k++) {
                                const T x = a[j + k];
                                const T y = a[j + i + k];
                                a[j + k] = x + y;
                                a[j + i + k] = (x - y) * z;
                        }
                        z *= nth_root.get_inv_rate(__builtin_popcount(j & ~(j + (i << 1))));
                }
        }
        T inv_n = T(1) / T(n);
        for (auto& x : a) x *= inv_n;
}

template <class T>
std::vector<T> convolution_naive(const std::vector<T>& a, const std::vector<T>& b) {
        int n = (int)a.size(), m = (int)b.size();
        std::vector<T> c(n + m - 1);
        for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) c[i + j] += a[i] * b[j];
        }
        return c;
}

template <class T>
std::vector<T> convolution_pow2(std::vector<T> a) {
        int n = (int)a.size() * 2 - 1;
        int lg = msb(n - 1) + 1;
        if (n - (1 << (lg - 1)) <= 5) {
                --lg;
                int m = (int)a.size() - (1 << (lg - 1));
                std::vector<T> a1(a.begin(), a.begin() + m);
                std::vector<T> a2(a.begin() + m, a.end());
                std::vector<T> c(n);
                std::vector<T> c1 = convolution_naive(a1, a1);
                std::vector<T> c2 = convolution_naive(a1, a2);
                std::vector<T> c3 = convolution_pow2(a2);
                for (int i = 0; i < (int)c1.size(); i++) c[i] += c1[i];
                for (int i = 0; i < (int)c2.size(); i++) c[i + m] += c2[i] * 2;
                for (int i = 0; i < (int)c3.size(); i++) c[i + m * 2] += c3[i];
                return c;
        }
        int m = 1 << lg;
        a.resize(m);
        number_theoretic_transform(a);
        for (int i = 0; i < m; i++) a[i] *= a[i];
        inverse_number_theoretic_transform(a);
        a.resize(n);
        return a;
}

template <class T>
std::vector<T> convolution(std::vector<T> a, std::vector<T> b) {
        int n = (int)a.size() + (int)b.size() - 1;
        int lg = ceil_log2(n);
        int m = 1 << lg;
        if (n - (1 << (lg - 1)) <= 5) {
                --lg;
                if (a.size() < b.size()) std::swap(a, b);
                int half_m = n - (1 << lg);
                std::vector<T> a1(a.begin(), a.begin() + half_m);
                std::vector<T> a2(a.begin() + half_m, a.end());
                std::vector<T> c(n);
                std::vector<T> c1 = convolution_naive(a1, b);
                std::vector<T> c2 = convolution(a2, b);
                for (int i = 0; i < (int)c1.size(); i++) c[i] += c1[i];
                for (int i = 0; i < (int)c2.size(); i++) c[i + half_m] += c2[i];
                return c;
        }
        a.resize(m);
        b.resize(m);
        number_theoretic_transform(a);
        number_theoretic_transform(b);
        for (int i = 0; i < m; i++) a[i] *= b[i];
        inverse_number_theoretic_transform(a);
        a.resize(n);
        return a;
}

} // namespace internal

template <uint32_t p>
struct is_ntt_friendly : std::false_type {};
template <> struct is_ntt_friendly<998244353> : std::true_type {};
template <> struct is_ntt_friendly<469762049> : std::true_type {};
template <> struct is_ntt_friendly<1811939329> : std::true_type {};
template <> struct is_ntt_friendly<2013265921> : std::true_type {};

template <uint32_t p>
std::vector<StaticModInt<p>> convolution_for_any_mod(const std::vector<StaticModInt<p>>& a, const std::vector<StaticModInt<p>>& b);

template <uint32_t p>
std::vector<StaticModInt<p>> convolution(const std::vector<StaticModInt<p>>& a, const std::vector<StaticModInt<p>>& b) {
        int n = (int)a.size(), m = (int)b.size();
        if (n == 0 || m == 0) return {};
        if (n <= 60 || m <= 60) return internal::convolution_naive(a, b);
        if (n + m - 1 <= (int)((1 - p) & (p - 1))) {
                if (n == m && a == b) return internal::convolution_pow2(a);
                return internal::convolution(a, b);
        }
        return convolution_for_any_mod(a, b);
}

template <uint32_t p>
std::vector<StaticModInt<p>> convolution_for_any_mod(const std::vector<StaticModInt<p>>& a, const std::vector<StaticModInt<p>>& b) {
        int n = (int)a.size(), m = (int)b.size();
        assert(n + m - 1 <= (1 << 26));
        static constexpr uint32_t MOD1 = 469762049;
        static constexpr uint32_t MOD2 = 1811939329;
        static constexpr uint32_t MOD3 = 2013265921;
        using mint1 = StaticModInt<MOD1>;
        using mint2 = StaticModInt<MOD2>;
        using mint3 = StaticModInt<MOD3>;
        std::vector<mint1> a1(n), b1(m);
        std::vector<mint2> a2(n), b2(m);
        std::vector<mint3> a3(n), b3(m);
        for (int i = 0; i < n; i++) {
                uint32_t v = a[i].get();
                a1[i] = v; a2[i] = v; a3[i] = v;
        }
        for (int i = 0; i < m; i++) {
                uint32_t v = b[i].get();
                b1[i] = v; b2[i] = v; b3[i] = v;
        }
        auto c1 = internal::convolution(a1, b1);
        auto c2 = internal::convolution(a2, b2);
        auto c3 = internal::convolution(a3, b3);
        static constexpr uint32_t INV1_2 = mint2(MOD1).inv().get();
        static constexpr uint32_t INV1_3 = mint3(MOD1).inv().get();
        static constexpr uint32_t INV2_3 = mint3(MOD2).inv().get();
        std::vector<StaticModInt<p>> res(n + m - 1);
        for (int i = 0; i < n + m - 1; i++) {
                int64_t t1 = c1[i].get();
                int64_t t2 = (c2[i].get() - t1 + MOD2) * INV1_2 % MOD2;
                if (t2 < 0) t2 += MOD2;
                int64_t t3 = ((c3[i].get() - t1 + MOD3) * INV1_3 % MOD3 - t2 + MOD3) * INV2_3 % MOD3;
                if (t3 < 0) t3 += MOD3;
                res[i] = StaticModInt<p>(t1 + (t2 + t3 * MOD2) % p * MOD1);
        }
        return res;
}

template <uint32_t p>
std::vector<int64_t> convolution(const std::vector<int64_t>& a, const std::vector<int64_t>& b) {
        int n = (int)a.size(), m = (int)b.size();
        std::vector<StaticModInt<p>> a2(n), b2(m);
        for (int i = 0; i < n; i++) a2[i] = a[i];
        for (int i = 0; i < m; i++) b2[i] = b[i];
        auto c2 = convolution<p>(a2, b2);
        std::vector<int64_t> c(c2.size());
        for (int i = 0; i < (int)c2.size(); i++) c[i] = c2[i].get();
        return c;
}

template <class T>
void ntt_doubling_(std::vector<T>& a, std::vector<T> b) {
        static constexpr internal::NthRoot<T> nth_root;
        int n = (int)a.size();
        const T z = nth_root.get(internal::msb(n) + 1);
        T r = 1;
        for (int i = 0; i < n; i++) {
                b[i] *= r;
                r *= z;
        }
        internal::number_theoretic_transform(b);
        a.reserve(2 * n);
        a.insert(a.end(), b.begin(), b.end());
}

template <class T>
void ntt_doubling_(std::vector<T>& a) {
        auto b = a;
        internal::inverse_number_theoretic_transform(b);
        ntt_doubling_(a, std::move(b));
}

template <int ID>
std::vector<DynamicModInt<ID>> convolution(const std::vector<DynamicModInt<ID>>& a, const std::vector<DynamicModInt<ID>>& b) {
        int n = (int)a.size(), m = (int)b.size();
        if (n == 0 || m == 0) return {};
        if (n <= 60 || m <= 60) return internal::convolution_naive(a, b);
        uint32_t mod = DynamicModInt<ID>::get_mod();
        if (n + m - 1 <= (int)((1 - mod) & (mod - 1))) {
                if (n == m && a == b) return internal::convolution_pow2(a);
                return internal::convolution(a, b);
        }
        static constexpr uint32_t MOD1 = 469762049;
        static constexpr uint32_t MOD2 = 1811939329;
        static constexpr uint32_t MOD3 = 2013265921;
        using mint1 = StaticModInt<MOD1>;
        using mint2 = StaticModInt<MOD2>;
        using mint3 = StaticModInt<MOD3>;
        std::vector<mint1> a1(n), b1(m);
        std::vector<mint2> a2(n), b2(m);
        std::vector<mint3> a3(n), b3(m);
        for (int i = 0; i < n; i++) {
                uint32_t v = a[i].get();
                a1[i] = v; a2[i] = v; a3[i] = v;
        }
        for (int i = 0; i < m; i++) {
                uint32_t v = b[i].get();
                b1[i] = v; b2[i] = v; b3[i] = v;
        }
        auto c1 = internal::convolution(a1, b1);
        auto c2 = internal::convolution(a2, b2);
        auto c3 = internal::convolution(a3, b3);
        static constexpr uint32_t INV1_2 = mint2(MOD1).inv().get();
        static constexpr uint32_t INV1_3 = mint3(MOD1).inv().get();
        static constexpr uint32_t INV2_3 = mint3(MOD2).inv().get();
        std::vector<DynamicModInt<ID>> res(n + m - 1);
        for (int i = 0; i < n + m - 1; i++) {
                int64_t t1 = c1[i].get();
                int64_t t2 = (c2[i].get() - t1 + MOD2) * INV1_2 % MOD2;
                if (t2 < 0) t2 += MOD2;
                int64_t t3 = ((c3[i].get() - t1 + MOD3) * INV1_3 % MOD3 - t2 + MOD3) * INV2_3 % MOD3;
                if (t3 < 0) t3 += MOD3;
                res[i] = DynamicModInt<ID>(t1 + (t2 + t3 * MOD2) % mod * MOD1);
        }
        return res;
}

} // namespace Convolution
