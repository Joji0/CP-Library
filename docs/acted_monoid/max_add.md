---
title: MaxAdd
documentation_of: acted_monoid/max_add.hpp
---

# MaxAdd — Range Add, Range Max

## Overview

ActedMonoid for lazy segment tree supporting **range add** updates and **range max** queries.

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `T` (default `int64_t`) | Max value |
| $F$ | `T` | Add value |
| $\mathrm{op}(a, b)$ | $\max(a, b)$ | Merge |
| $e$ | `std::numeric_limits<T>::min()` | Identity |
| $\mathrm{mapping}(f, x)$ | $x + f$ | Add $f$ to value |
| $\mathrm{composition}(f, g)$ | $f + g$ | Combine two adds |
| $\mathrm{id}$ | $0$ | No-op |

## Usage

```cpp
#include "acted_monoid/max_add.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<int64_t> a = {3, 1, 4, 1, 5};
LazySegTree<MaxAdd<>> seg(a);

seg.query(0, 4);       // 5
seg.update(0, 2, 10LL); // add 10 to [0..2]
seg.query(0, 4);       // 14 (max of 13,11,14,1,5)
```

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MaxAdd {
        using S = T;
        using F = T;
        static S op(S a, S b) { return std::max(a, b); }
        static S e() { return std::numeric_limits<T>::min(); }
        static S mapping(F f, S x) { return x + f; }
        static F composition(F f, F g) { return f + g; }
        static F id() { return 0; }
};
```
