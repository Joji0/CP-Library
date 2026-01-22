#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

template <typename Mint> struct Combinatorics {
        struct Storage {
                std::vector<Mint> fact, inv, invfact, der;
                Storage() {
                        fact = {Mint(1), Mint(1)};
                        inv = {Mint(0), Mint(1)};
                        invfact = {Mint(1), Mint(1)};
                        der = {Mint(1), Mint(0)};
                }
                void extend(int n) {
                        if (n < (int)fact.size()) return;
                        int target = std::max(n + 1, (int)fact.size() * 2);
                        int curr = (int)fact.size();
                        int MOD = Mint::mod();
                        fact.resize(target);
                        inv.resize(target);
                        invfact.resize(target);
                        der.resize(target);
                        for (int i = curr; i < target; i++) {
                                fact[i] = fact[i - 1] * Mint(i);
                                if (i < MOD) {
                                        inv[i] = inv[MOD % i] * Mint(MOD - MOD / i);
                                } else {
                                        inv[i] = Mint(i).inverse();
                                }
                                invfact[i] = invfact[i - 1] * inv[i];
                                der[i] = Mint(i - 1) * (der[i - 1] + der[i - 2]);
                        }
                }
        };
        static Storage &data() {
                static Storage s;
                return s;
        }
        static void ensure(int n) { data().extend(n); }
        static Mint fact(int n) {
                if (n < 0) return Mint(0);
                ensure(n);
                return data().fact[n];
        }
        static Mint inv(int n) {
                if (n < 0) return Mint(n).inverse();
                ensure(n);
                return data().inv[n];
        }
        static Mint invfact(int n) {
                if (n < 0) return Mint(0);
                ensure(n);
                return data().invfact[n];
        }
        static Mint derangement(int n) {
                if (n < 0) return Mint(0);
                ensure(n);
                return data().der[n];
        }
        static Mint C(int64_t n, int64_t k) {
                if (k < 0 || k > n) return Mint(0);
                if (n < (int64_t)data().fact.size()) {
                        return fact(n) * invfact(k) * invfact(n - k);
                }
                if (n < 20000000) {
                        ensure(n);
                        return fact(n) * invfact(k) * invfact(n - k);
                }
                return C_large(n, k);
        }
        static Mint C_large(int64_t n, int64_t k) {
                if (k < 0 || k > n) return Mint(0);
                k = std::min(k, n - k);
                Mint num(1);
                for (int64_t i = 0; i < k; i++) {
                        num *= Mint(n - i);
                }
                return num * invfact(k);
        }
        static Mint P(int64_t n, int64_t k) {
                if (k < 0 || k > n) return Mint(0);
                if (n < 20000000) {
                        ensure(n);
                        return fact(n) * invfact(n - k);
                }
                Mint res(1);
                for (int64_t i = 0; i < k; i++) {
                        res *= Mint(n - i);
                }
                return res;
        }
        static Mint H(int64_t n, int64_t k) {
                if (n < 0 || k < 0) return Mint(0);
                if (n == 0 && k == 0) return Mint(1);
                return C(n + k - 1, k);
        }
        static Mint stars_and_bars(int64_t n, int64_t k, bool allow_empty = true) {
                if (k <= 0) return (n == 0 && k == 0) ? Mint(1) : Mint(0);
                if (allow_empty) {
                        return C(n + k - 1, k - 1);
                } else {
                        if (n < k) return Mint(0);
                        return C(n - 1, k - 1);
                }
        }
        static Mint catalan(int n) {
                if (n < 0) return Mint(0);
                return C(2 * n, n) * inv(n + 1);
        }
        static Mint multinomial(int n, const std::vector<int> &ks) {
                if (n < 0) return Mint(0);
                int64_t sum = 0;
                Mint denom(1);
                for (int k : ks) {
                        if (k < 0) return Mint(0);
                        sum += k;
                        denom *= invfact(k);
                }
                if (sum != n) return Mint(0);
                return fact(n) * denom;
        }
};
