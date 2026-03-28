---
title: MulMonoid
documentation_of: //monoid/mul.hpp
---

## Overview

Monoid for **range product** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `T` (default `int64_t`) |
| Operation $\cdot$ | $a \times b$ |
| Identity $e$ | $1$ |

**Warning**: May overflow for large values. Consider using with `ModInt` for modular products.

## Usage

```cpp
#include "monoid/mul.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<MulMonoid<>> seg(n); // range product, point update
```
