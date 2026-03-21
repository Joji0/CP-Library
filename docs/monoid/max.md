---
title: MaxMonoid
documentation_of: monoid/max.hpp
---

# MaxMonoid

## Overview

Monoid for **range maximum** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `long long` |
| Operation $\cdot$ | $\max(a, b)$ |
| Identity $e$ | $-10^{18}$ |

## Usage

```cpp
#include "monoid/max.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<MaxMonoid> seg(n); // range max, point update
```
