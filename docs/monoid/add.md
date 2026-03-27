---
title: AddMonoid
documentation_of: monoid/add.hpp
---

# AddMonoid

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

## Source Code

```cpp
#pragma once
#include <cstdint>

template <typename T = int64_t> struct AddMonoid {
        using value_type = T;
        static value_type e() { return 0; }
        static value_type op(value_type a, value_type b) { return a + b; }
};
```
