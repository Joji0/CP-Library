---
title: MinAdd
documentation_of: acted_monoid/min_add.hpp
---

# MinAdd — Range Add, Range Min

## Overview

ActedMonoid for lazy segment tree supporting **range add** updates and **range min** queries.

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `T` (default `int64_t`) | Min value |
| $F$ | `T` | Add value |
| $\mathrm{op}(a, b)$ | $\min(a, b)$ | Merge |
| $e$ | `std::numeric_limits<T>::max()` | Identity |
| $\mathrm{mapping}(f, x)$ | $x + f$ | Add $f$ to value |
| $\mathrm{composition}(f, g)$ | $f + g$ | Combine two adds |
| $\mathrm{id}$ | $0$ | No-op |

## Usage

```cpp
#include "acted_monoid/min_add.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<int64_t> a = {3, 1, 4, 1, 5};
LazySegTree<MinAdd<>> seg(a);

seg.query(0, 4);        // 1
seg.update(0, 4, 10LL); // add 10 to all
seg.query(0, 4);        // 11
```

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MinAdd {
        using S = T;
        using F = T;
        static S op(S a, S b) { return std::min(a, b); }
        static S e() { return std::numeric_limits<T>::max(); }
        static S mapping(F f, S x) { return x + f; }
        static F composition(F f, F g) { return f + g; }
        static F id() { return 0; }
};
```
