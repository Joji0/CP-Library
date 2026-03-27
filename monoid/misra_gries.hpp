#pragma once
#include <algorithm>
#include <array>

// https://codeforces.com/problemset/problem/2149/G
template <int K> struct MisraGriesMonoid {
        struct Node {
                std::array<int, K> c;
                std::array<int, K> v;
                Node() {
                        c.fill(0);
                        v.fill(0);
                }
                Node(int val) {
                        c.fill(0);
                        v.fill(0);
                        if (K > 0) {
                                c[0] = val;
                                v[0] = 1;
                        }
                }
        };
        using value_type = Node;
        static value_type e() { return Node(); }
        static value_type op(const value_type &a, const value_type &b) {
                value_type res = a;
                for (int i = 0; i < K; ++i) {
                        if (b.v[i] == 0) continue;
                        int x = b.c[i];
                        int v = b.v[i];
                        bool matched = false;
                        for (int j = 0; j < K; ++j) {
                                if (res.v[j] > 0 && res.c[j] == x) {
                                        res.v[j] += v;
                                        matched = true;
                                        break;
                                }
                        }
                        if (matched) continue;
                        bool placed = false;
                        for (int j = 0; j < K; ++j) {
                                if (res.v[j] == 0) {
                                        res.c[j] = x;
                                        res.v[j] = v;
                                        placed = true;
                                        break;
                                }
                        }
                        if (placed) continue;
                        int mn = v;
                        for (int j = 0; j < K; ++j) {
                                mn = std::min(mn, res.v[j]);
                        }
                        for (int j = 0; j < K; ++j) {
                                res.v[j] -= mn;
                        }
                        v -= mn;
                        if (v > 0) {
                                for (int j = 0; j < K; ++j) {
                                        if (res.v[j] == 0) {
                                                res.c[j] = x;
                                                res.v[j] = v;
                                                break;
                                        }
                                }
                        }
                }
                return res;
        }
};
