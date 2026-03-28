---
title: MaxMonoid
documentation_of: //monoid/max.hpp
---

## Overview

Monoid for **range maximum** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `T` (default `int64_t`) |
| Operation $\cdot$ | $\max(a, b)$ |
| Identity $e$ | `std::numeric_limits<T>::min()` |

## Usage

```cpp
#include "monoid/max.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<MaxMonoid<>> seg(n); // range max, point update
```
