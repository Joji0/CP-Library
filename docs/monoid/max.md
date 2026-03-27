---
title: MaxMonoid
documentation_of: monoid/max.hpp
---

# MaxMonoid

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

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = int64_t> struct MaxMonoid {
        using value_type = T;
        static value_type e() { return std::numeric_limits<T>::min(); }
        static value_type op(value_type a, value_type b) { return std::max(a, b); }
};
```
