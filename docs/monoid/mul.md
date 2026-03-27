---
title: MulMonoid
documentation_of: monoid/mul.hpp
---

# MulMonoid

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

## Source Code

```cpp
#pragma once
#include <cstdint>

template <typename T = int64_t> struct MulMonoid {
        using value_type = T;
        static value_type e() { return 1; }
        static value_type op(value_type a, value_type b) { return a * b; }
};
```
