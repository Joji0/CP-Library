---
title: MinMonoid
documentation_of: monoid/min.hpp
---

# MinMonoid

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

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MinMonoid {
        using value_type = T;
        static value_type e() { return std::numeric_limits<T>::max(); }
        static value_type op(value_type a, value_type b) { return std::min(a, b); }
};
```
