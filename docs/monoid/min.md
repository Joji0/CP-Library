---
title: MinMonoid
documentation_of: monoid/min.hpp
---

# MinMonoid

## Overview

Monoid for **range minimum** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `int64_t` |
| Operation $\cdot$ | $\min(a, b)$ |
| Identity $e$ | $10^{18}$ |

## Usage

```cpp
#include "monoid/min.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<MinMonoid> seg(n); // range min, point update
```

## Source Code

```cpp
#pragma once
#include <algorithm>

struct MinMonoid {
	using value_type = int64_t;
	static value_type e() { return (int64_t)1e18; }
	static value_type op(value_type a, value_type b) { return std::min(a, b); }
};

```
