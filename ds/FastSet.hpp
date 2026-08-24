#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <cassert>

class FastSet {
        int n;
        std::vector<std::vector<uint64_t>> seg;
public:
        explicit FastSet(int n_) : n(n_) {
                int d = 1;
                while (true) {
                        d++;
                        n_ = (n_ + 63) / 64;
                        if (n_ <= 1) break;
                }
                seg.resize(d);
                n_ = n;
                for (int i = 0; i < d; ++i) {
                        seg[i].assign((n_ + 63) / 64, 0);
                        n_ = (n_ + 63) / 64;
                }
        }
        void build(const std::string& s) {
                assert((int)s.size() == n);
                for (int i = 0; i < n; ++i) {
                        if (s[i] == '1') {
                                seg[0][i >> 6] |= (1ULL << (i & 63));
                        }
                }
                for (int i = 0; i < (int)seg.size() - 1; ++i) {
                        for (int j = 0; j < (int)seg[i].size(); ++j) {
                                if (seg[i][j]) {
                                        seg[i + 1][j >> 6] |= (1ULL << (j & 63));
                                }
                        }
                }
        }
        bool contains(int i) const {
                if (i < 0 || i >= n) return false;
                return (seg[0][i >> 6] >> (i & 63)) & 1;
        }
        void insert(int i) {
                for (int h = 0; h < (int)seg.size(); ++h) {
                        seg[h][i >> 6] |= (1ULL << (i & 63));
                        i >>= 6;
                }
        }
        void erase(int i) {
                for (int h = 0; h < (int)seg.size(); ++h) {
                        seg[h][i >> 6] &= ~(1ULL << (i & 63));
                        if (seg[h][i >> 6]) break;
                        i >>= 6;
                }
        }
        int next(int i) const {
                if (i < 0) i = 0;
                if (i >= n) return -1;
                for (int h = 0; h < (int)seg.size(); ++h) {
                        if (i >> 6 >= (int)seg[h].size()) break;
                        uint64_t d = seg[h][i >> 6];
                        d &= (~0ULL) << (i & 63);
                        if (!d) {
                                i = (i >> 6) + 1;
                                continue;
                        }
                        i = (i >> 6 << 6) + __builtin_ctzll(d);
                        for (int g = h - 1; g >= 0; --g) {
                                i = (i << 6) + __builtin_ctzll(seg[g][i]);
                        }
                        return i < n ? i : -1;
                }
                return -1;
        }
        int prev(int i) const {
                if (i < 0) return -1;
                if (i >= n) i = n - 1;
                for (int h = 0; h < (int)seg.size(); ++h) {
                        if (i >> 6 >= (int)seg[h].size()) break;
                        uint64_t d = seg[h][i >> 6];
                        d &= (~0ULL) >> (63 - (i & 63));
                        if (!d) {
                                i = (i >> 6) - 1;
                                if (i < 0) return -1;
                                continue;
                        }
                        i = (i >> 6 << 6) + (63 - __builtin_clzll(d));
                        for (int g = h - 1; g >= 0; --g) {
                                i = (i << 6) + (63 - __builtin_clzll(seg[g][i]));
                        }
                        return i;
                }
                return -1;
        }
        bool empty() const {
                return seg.back().empty() || seg.back()[0] == 0ULL;
        }
        int min() const {
                return next(0);
        }
        int max() const {
                return prev(n - 1);
        }
        bool any(int l, int r) const {
                int x = next(l);
                return x != -1 && x < r;
        }
        template <typename F>
        void enumerate(int l, int r, F f) const {
                for (int x = next(l); x != -1 && x < r; x = next(x + 1)) {
                        f(x);
                }
        }
        void clear() {
                for (auto& v : seg) {
                        std::fill(v.begin(), v.end(), 0ULL);
                }
        }
};
