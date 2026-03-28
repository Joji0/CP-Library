---
title: AddMonoid
documentation_of: //monoid/add.hpp
---

## Overview

Monoid for **range sum** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `T` (default `int64_t`) |
| Operation $\cdot$ | $a + b$ |
| Identity $e$ | $0$ |

## Usage

```cpp
#include "monoid/add.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<AddMonoid<>> seg(n);    // range sum, point update (int64_t)
SegTree<AddMonoid<int>> seg(n); // range sum with int
```
