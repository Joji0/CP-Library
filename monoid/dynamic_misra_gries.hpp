#pragma once
#include <algorithm>
#include <array>

//https://codeforces.com/contest/840/problem/D
template <int MAX_K> struct DynamicMisraGriesMonoid {
        struct Node {
                std::array<int, MAX_K> c;
                std::array<int, MAX_K> v;
                int k;
                Node() : k(0) {
                        c.fill(0);
                        v.fill(0);
                }
                Node(int val, int _k) : k(_k) {
                        c.fill(0);
                        v.fill(0);
                        if (k > 0) {
                                c[0] = val;
                                v[0] = 1;
                        }
                }
        };
        using value_type = Node;
        static value_type e() { return Node(); }
        static value_type op(const value_type &a, const value_type &b) {
                if (a.k == 0) return b;
                if (b.k == 0) return a;
                value_type res = a;
                int current_k = std::max(a.k, b.k);
                res.k = current_k;
                for (int i = 0; i < b.k; ++i) {
                        if (b.v[i] == 0) continue;
                        int x = b.c[i];
                        int vol = b.v[i];
                        bool matched = false;
                        for (int j = 0; j < current_k; ++j) {
                                if (res.v[j] > 0 && res.c[j] == x) {
                                        res.v[j] += vol;
                                        matched = true;
                                        break;
                                }
                        }
                        if (matched) continue;
                        bool placed = false;
                        for (int j = 0; j < current_k; ++j) {
                                if (res.v[j] == 0) {
                                        res.c[j] = x;
                                        res.v[j] = vol;
                                        placed = true;
                                        break;
                                }
                        }
                        if (placed) continue;
                        int mn = vol;
                        for (int j = 0; j < current_k; ++j) {
                                mn = std::min(mn, res.v[j]);
                        }
                        for (int j = 0; j < current_k; ++j) {
                                res.v[j] -= mn;
                        }
                        vol -= mn;
                        if (vol > 0) {
                                for (int j = 0; j < current_k; ++j) {
                                        if (res.v[j] == 0) {
                                                res.c[j] = x;
                                                res.v[j] = vol;
                                                break;
                                        }
                                }
                        }
                }
                return res;
        }
};
