---
title: MinAffine
documentation_of: acted_monoid/min_affine.hpp
---

# MinAffine — Range Affine, Range Min

## Overview

ActedMonoid for lazy segment tree supporting **range affine transformation** $x \mapsto ax + b$ and **range min** queries.

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `T` (default `int64_t`) | Min value |
| $F$ | `struct {T a, b;}` | Affine: $x \mapsto ax + b$ |
| $\mathrm{op}(a, b)$ | $\min(a, b)$ | Merge |
| $e$ | `std::numeric_limits<T>::max()` | Identity |
| $\mathrm{mapping}(f, x)$ | $f_a \cdot x + f_b$ | Apply affine |
| $\mathrm{composition}(f, g)$ | $\{f_a \cdot g_a,\; f_a \cdot g_b + f_b\}$ | $f \circ g$ |
| $\mathrm{id}$ | $\{1, 0\}$ | Identity |

**Warning**: Same as [MaxAffine](./max_affine.md) — only correct when $f_a \ge 0$.

## Usage

```cpp
#include "acted_monoid/min_affine.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<int64_t> a = {3, 1, 4};
LazySegTree<MinAffine<>> seg(a);

seg.update(0, 2, MinAffine<>::F{2, 1}); // 2x+1
seg.query(0, 2);                         // 3 (min of 7,3,9)
```

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MinAffine {
        using S = T;
        struct F {
                T a, b;
        };
        static S op(S a, S b) { return std::min(a, b); }
        static S e() { return std::numeric_limits<T>::max(); }
        static S mapping(F f, S x) { return f.a * x + f.b; }
        static F composition(F f, F g) { return {f.a * g.a, f.a * g.b + f.b}; }
        static F id() { return {1, 0}; }
};
```
