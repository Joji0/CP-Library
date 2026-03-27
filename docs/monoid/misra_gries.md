---
title: MisraGriesMonoid
documentation_of: monoid/misra_gries.hpp
---

# MisraGriesMonoid

## Overview

Monoid implementing the [Misra-Gries](https://en.wikipedia.org/wiki/Misra%E2%80%93Gries_summary) heavy hitter summary algorithm. When used with a segment tree, it can answer range queries for elements that appear more than $\lfloor n / K \rfloor$ times in a subarray.

The summary maintains at most $K$ candidate elements with their approximate frequencies. After querying a range, any element appearing more than $\lfloor \text{len} / (K+1) \rfloor$ times is guaranteed to be among the $K$ candidates (but candidates may be false positives).

| Property | Value |
|----------|-------|
| Set $S$ | `Node` (arrays of $K$ candidates and counts) |
| Operation $\cdot$ | Merge two summaries |
| Identity $e$ | Empty summary (all counts $0$) |

## Template Parameter

| Parameter | Description |
|-----------|-------------|
| `K` | Maximum number of candidates to track. Use $K = 1$ for majority element, $K = 2$ for elements appearing more than $n/3$ times, etc. |

## Node Structure

```cpp
struct Node {
    std::array<int, K> c; // candidate values
    std::array<int, K> v; // candidate counts
};
```

## Usage

```cpp
#include "monoid/misra_gries.hpp"
#include "ds/segtree/segtree.hpp"

using MG = MisraGriesMonoid<1>; // track majority element
std::vector<MG::value_type> a;
for (int x : values) a.push_back(MG::value_type(x));
SegTree<MG> seg(a);

auto res = seg.query(l, r);
// res.c[0] is the majority candidate, res.v[0] is its approx count
// verify by counting actual occurrences if needed
```

## Source Code

```cpp
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
```
