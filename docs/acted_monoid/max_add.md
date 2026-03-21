---
title: MaxAdd
documentation_of: acted_monoid/max_add.hpp
---

# MaxAdd — Range Add, Range Max

## Overview

ActedMonoid for lazy segment tree supporting **range add** updates and **range max** queries.

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `long long` | Max value |
| $F$ | `long long` | Add value |
| $\mathrm{op}(a, b)$ | $\max(a, b)$ | Merge |
| $e$ | $-10^{18}$ | Identity |
| $\mathrm{mapping}(f, x)$ | $x + f$ | Add $f$ to value |
| $\mathrm{composition}(f, g)$ | $f + g$ | Combine two adds |
| $\mathrm{id}$ | $0$ | No-op |

## Usage

```cpp
#include "acted_monoid/max_add.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<long long> a = {3, 1, 4, 1, 5};
LazySegTree<MaxAdd> seg(a);

seg.query(0, 4);       // 5
seg.update(0, 2, 10LL); // add 10 to [0..2]
seg.query(0, 4);       // 14 (max of 13,11,14,1,5)
```
