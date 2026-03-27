---
title: GcdMonoid
documentation_of: monoid/gcd.hpp
---

# GcdMonoid

## Overview

Monoid for **range GCD** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `T` (default `int64_t`) |
| Operation $\cdot$ | $\gcd(a, b)$ |
| Identity $e$ | $0$ (since $\gcd(a, 0) = a$) |

## Usage

```cpp
#include "monoid/gcd.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<GcdMonoid<>> seg(n); // range gcd, point update
```

## Source Code

```cpp
#pragma once
#include <cstdint>
#include <numeric>

template <typename T = int64_t> struct GcdMonoid {
        using value_type = T;
        static value_type e() { return 0; }
        static value_type op(value_type a, value_type b) { return std::gcd(a, b); }
};
```
