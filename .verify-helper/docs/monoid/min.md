---
title: MinMonoid
documentation_of: //monoid/min.hpp
---

## Overview

Monoid for **range minimum** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `T` (default `int64_t`) |
| Operation $\cdot$ | $\min(a, b)$ |
| Identity $e$ | `std::numeric_limits<T>::max()` |

## Usage

```cpp
#include "monoid/min.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<MinMonoid<>> seg(n); // range min, point update
```
