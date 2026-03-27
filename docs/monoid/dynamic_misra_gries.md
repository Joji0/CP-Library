---
title: DynamicMisraGriesMonoid
documentation_of: monoid/dynamic_misra_gries.hpp
---

# DynamicMisraGriesMonoid

## Overview

A variant of [MisraGriesMonoid](./misra_gries.md) where each element can specify its own $k$ value (number of candidates to track). This allows different nodes in the segment tree to use different summary sizes, with the merge operation taking the maximum $k$.

Useful when the number of candidates varies per query or when building from elements with different tracking requirements.

| Property | Value |
|----------|-------|
| Set $S$ | `Node` (arrays of up to `MAX_K` candidates and counts, with dynamic $k$) |
| Operation $\cdot$ | Merge two summaries using $\max(k_1, k_2)$ candidates |
| Identity $e$ | Empty summary ($k = 0$) |

## Template Parameter

| Parameter | Description |
|-----------|-------------|
| `MAX_K` | Maximum possible $k$ value. Determines the fixed array size for candidates. |

## Node Structure

```cpp
struct Node {
    std::array<int, MAX_K> c; // candidate values
    std::array<int, MAX_K> v; // candidate counts
    int k;                     // current number of candidates to track
};
```

A `Node` can be constructed with `Node(val, k)` to create a single-element summary tracking $k$ candidates.

## Usage

```cpp
#include "monoid/dynamic_misra_gries.hpp"
#include "ds/segtree/segtree.hpp"

using DMG = DynamicMisraGriesMonoid<3>; // max 3 candidates
std::vector<DMG::value_type> a;
int k = 2; // track 2 candidates per element
for (int x : values) a.push_back(DMG::value_type(x, k));
SegTree<DMG> seg(a);

auto res = seg.query(l, r);
// res.c[0..res.k-1] are candidates, res.v[0..res.k-1] are counts
```

## Source Code

```cpp
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
```
