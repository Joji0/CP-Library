---
title: MinAdd
documentation_of: acted_monoid/min_add.hpp
---

# MinAdd — Range Add, Range Min

## Overview

ActedMonoid for lazy segment tree supporting **range add** updates and **range min** queries.

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `long long` | Min value |
| $F$ | `long long` | Add value |
| $\mathrm{op}(a, b)$ | $\min(a, b)$ | Merge |
| $e$ | $10^{18}$ | Identity |
| $\mathrm{mapping}(f, x)$ | $x + f$ | Add $f$ to value |
| $\mathrm{composition}(f, g)$ | $f + g$ | Combine two adds |
| $\mathrm{id}$ | $0$ | No-op |

## Usage

```cpp
#include "acted_monoid/min_add.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<long long> a = {3, 1, 4, 1, 5};
LazySegTree<MinAdd> seg(a);

seg.query(0, 4);        // 1
seg.update(0, 4, 10LL); // add 10 to all
seg.query(0, 4);        // 11
```
