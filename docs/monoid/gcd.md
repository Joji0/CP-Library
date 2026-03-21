---
title: GcdMonoid
documentation_of: monoid/gcd.hpp
---

# GcdMonoid

## Overview

Monoid for **range GCD** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `long long` |
| Operation $\cdot$ | $\gcd(a, b)$ |
| Identity $e$ | $0$ (since $\gcd(a, 0) = a$) |

## Usage

```cpp
#include "monoid/gcd.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<GcdMonoid> seg(n); // range gcd, point update
```
