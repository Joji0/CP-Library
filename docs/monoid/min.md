---
title: MinMonoid
documentation_of: monoid/min.hpp
---

# MinMonoid

## Overview

Monoid for **range minimum** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `long long` |
| Operation $\cdot$ | $\min(a, b)$ |
| Identity $e$ | $10^{18}$ |

## Usage

```cpp
#include "monoid/min.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<MinMonoid> seg(n); // range min, point update
```
