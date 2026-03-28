---
title: DynamicMisraGriesMonoid
documentation_of: //monoid/dynamic_misra_gries.hpp
---

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
