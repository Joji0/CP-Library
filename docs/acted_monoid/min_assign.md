---
title: MinAssign
documentation_of: acted_monoid/min_assign.hpp
---

# MinAssign — Range Assign, Range Min

## Overview

ActedMonoid for lazy segment tree supporting **range assign** and **range min** queries.

Uses `LLONG_MAX` as a sentinel for "no assignment".

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `long long` | Min value |
| $F$ | `long long` | Assign value (`LLONG_MAX` = no-op) |
| $\mathrm{op}(a, b)$ | $\min(a, b)$ | Merge |
| $e$ | $10^{18}$ | Identity |
| $\mathrm{mapping}(f, x)$ | $f = \mathrm{NONE}$ ? $x$ : $f$ | Assign |
| $\mathrm{composition}(f, g)$ | $f = \mathrm{NONE}$ ? $g$ : $f$ | Later wins |
| $\mathrm{id}$ | `LLONG_MAX` | No-op |

## Usage

```cpp
#include "acted_monoid/min_assign.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<long long> a = {3, 1, 4, 1, 5};
LazySegTree<MinAssign> seg(a);

seg.update(0, 2, 0LL); // assign 0 to [0..2]
seg.query(0, 4);        // 0
```
