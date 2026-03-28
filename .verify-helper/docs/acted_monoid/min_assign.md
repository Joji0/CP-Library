---
title: MinAssign
documentation_of: //acted_monoid/min_assign.hpp
---

## Overview

ActedMonoid for lazy segment tree supporting **range assign** and **range min** queries.

Uses `std::numeric_limits<T>::max()` as a sentinel for "no assignment".

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `T` (default `int64_t`) | Min value |
| $F$ | `T` | Assign value (`NONE` = no-op) |
| $\mathrm{op}(a, b)$ | $\min(a, b)$ | Merge |
| $e$ | `std::numeric_limits<T>::max()` | Identity |
| $\mathrm{mapping}(f, x)$ | $f = \mathrm{NONE}$ ? $x$ : $f$ | Assign |
| $\mathrm{composition}(f, g)$ | $f = \mathrm{NONE}$ ? $g$ : $f$ | Later wins |
| $\mathrm{id}$ | `NONE` | No-op |

## Usage

```cpp
#include "acted_monoid/min_assign.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<int64_t> a = {3, 1, 4, 1, 5};
LazySegTree<MinAssign<>> seg(a);

seg.update(0, 2, 0LL); // assign 0 to [0..2]
seg.query(0, 4);        // 0
```
