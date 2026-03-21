---
title: MaxAffine
documentation_of: acted_monoid/max_affine.hpp
---

# MaxAffine — Range Affine, Range Max

## Overview

ActedMonoid for lazy segment tree supporting **range affine transformation** $x \mapsto ax + b$ and **range max** queries.

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `long long` | Max value |
| $F$ | `struct {long long a, b;}` | Affine: $x \mapsto ax + b$ |
| $\mathrm{op}(a, b)$ | $\max(a, b)$ | Merge |
| $e$ | $-10^{18}$ | Identity |
| $\mathrm{mapping}(f, x)$ | $f_a \cdot x + f_b$ | Apply affine |
| $\mathrm{composition}(f, g)$ | $\{f_a \cdot g_a,\; f_a \cdot g_b + f_b\}$ | $f \circ g$ |
| $\mathrm{id}$ | $\{1, 0\}$ | Identity |

**Warning**: This is only correct when $f_a \ge 0$ (non-negative coefficient). If $f_a < 0$, the max/min relationship reverses and this acted monoid becomes invalid. For problems with negative coefficients, consider using a different approach.

## Usage

```cpp
#include "acted_monoid/max_affine.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<long long> a = {3, 1, 4};
LazySegTree<MaxAffine> seg(a);

seg.update(0, 2, MaxAffine::F{2, 1}); // 2x+1
seg.query(0, 2);                       // 9 (max of 7,3,9)
```
