---
title: MaxMonoid
documentation_of: monoid/max.hpp
---

# MaxMonoid

## Overview

Monoid for **range maximum** queries on a segment tree.

| Property | Value |
|----------|-------|
| Set $S$ | `int64_t` |
| Operation $\cdot$ | $\max(a, b)$ |
| Identity $e$ | $-10^{18}$ |

## Usage

```cpp
#include "monoid/max.hpp"
#include "ds/segtree/segtree.hpp"

SegTree<MaxMonoid> seg(n); // range max, point update
```

## Source Code

```cpp
#pragma once
#include <algorithm>

struct MaxMonoid {
	using value_type = int64_t;
	static value_type e() { return -(int64_t)1e18; }
	static value_type op(value_type a, value_type b) { return std::max(a, b); }
};

```
