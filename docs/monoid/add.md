---
title: AddMonoid
documentation_of: monoid/add.hpp
---

# AddMonoid

## Overview

Monoid for **range sum** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `long long` |
| Operation $\cdot$ | $a + b$ |
| Identity $e$ | $0$ |

## Usage

```cpp
#include "monoid/add.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<AddMonoid> seg(n); // range sum, point update
```
