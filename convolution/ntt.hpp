#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>

int primitive_root(int mod) {
        std::vector<int> factors;
        int x = mod - 1;
        for (int p = 2; (int64_t)p * p <= x; p++) {
                if (x % p == 0) {
                        factors.push_back(p);
                        while (x % p == 0) x /= p;
                }
        }
        if (x > 1) factors.push_back(x);
        for (int g = 2;; g++) {
                bool ok = true;
                for (int p : factors) {
                        int64_t res = 1, a = g;
                        int e = (mod - 1) / p;
                        while (e > 0) {
                                if (e & 1) res = res * a % mod;
                                a = a * a % mod;
                                e >>= 1;
                        }
                        if (res == 1) {
                                ok = false;
                                break;
                        }
                }
                if (ok) return g;
        }
}

template <typename Mint>
void ntt(std::vector<Mint> &a, bool invert) {
        int n = (int)a.size();
        for (int i = 1, j = 0; i < n; i++) {
                int bit = n >> 1;
                for (; j & bit; bit >>= 1) j ^= bit;
                j ^= bit;
                if (i < j) std::swap(a[i], a[j]);
        }
        Mint root = primitive_root((int)Mint::mod());
        for (int len = 2; len <= n; len <<= 1) {
                Mint wlen = root.pow((Mint::mod() - 1) / len);
                if (invert) wlen = wlen.inverse();
                for (int i = 0; i < n; i += len) {
                        Mint w = 1;
                        for (int j = 0; j < len / 2; j++) {
                                Mint u = a[i + j], v = a[i + j + len / 2] * w;
                                a[i + j] = u + v;
                                a[i + j + len / 2] = u - v;
                                w *= wlen;
                        }
                }
        }
        if (invert) {
                Mint inv_n = Mint(n).inverse();
                for (Mint &x : a) x *= inv_n;
        }
}

template <typename Mint>
std::vector<Mint> convolution(std::vector<Mint> a, std::vector<Mint> b) {
        if (a.empty() || b.empty()) return {};
        int need = (int)a.size() + (int)b.size() - 1;
        int n = 1;
        while (n < need) n <<= 1;
        a.resize(n);
        b.resize(n);
        ntt(a, false);
        ntt(b, false);
        for (int i = 0; i < n; i++) a[i] *= b[i];
        ntt(a, true);
        a.resize(need);
        return a;
}
