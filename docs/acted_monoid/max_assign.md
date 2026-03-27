---
title: MaxAssign
documentation_of: acted_monoid/max_assign.hpp
---

# MaxAssign — Range Assign, Range Max

## Overview

ActedMonoid for lazy segment tree supporting **range assign** and **range max** queries.

Uses `std::numeric_limits<T>::min()` as a sentinel for "no assignment".

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `T` (default `int64_t`) | Max value |
| $F$ | `T` | Assign value (`NONE` = no-op) |
| $\mathrm{op}(a, b)$ | $\max(a, b)$ | Merge |
| $e$ | `std::numeric_limits<T>::min()` | Identity |
| $\mathrm{mapping}(f, x)$ | $f = \mathrm{NONE}$ ? $x$ : $f$ | Assign |
| $\mathrm{composition}(f, g)$ | $f = \mathrm{NONE}$ ? $g$ : $f$ | Later wins |
| $\mathrm{id}$ | `NONE` | No-op |

## Usage

```cpp
#include "acted_monoid/max_assign.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<int64_t> a = {3, 1, 4, 1, 5};
LazySegTree<MaxAssign<>> seg(a);

seg.update(0, 2, 10LL); // assign 10 to [0..2]
seg.query(0, 4);         // 10
```

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MaxAssign {
        using S = T;
        using F = T;
        static constexpr F NONE = std::numeric_limits<T>::min();
        static S op(S a, S b) { return std::max(a, b); }
        static S e() { return std::numeric_limits<T>::min(); }
        static S mapping(F f, S x) { return f == NONE ? x : f; }
        static F composition(F f, F g) { return f == NONE ? g : f; }
        static F id() { return NONE; }
};
```
