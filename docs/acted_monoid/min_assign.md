---
title: MinAssign
documentation_of: acted_monoid/min_assign.hpp
---

# MinAssign — Range Assign, Range Min

## Overview

ActedMonoid for lazy segment tree supporting **range assign** and **range min** queries.

Uses `LLONG_MAX` as a sentinel for "no assignment".

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `int64_t` | Min value |
| $F$ | `int64_t` | Assign value (`LLONG_MAX` = no-op) |
| $\mathrm{op}(a, b)$ | $\min(a, b)$ | Merge |
| $e$ | $10^{18}$ | Identity |
| $\mathrm{mapping}(f, x)$ | $f = \mathrm{NONE}$ ? $x$ : $f$ | Assign |
| $\mathrm{composition}(f, g)$ | $f = \mathrm{NONE}$ ? $g$ : $f$ | Later wins |
| $\mathrm{id}$ | `LLONG_MAX` | No-op |

## Usage

```cpp
#include "acted_monoid/min_assign.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<int64_t> a = {3, 1, 4, 1, 5};
LazySegTree<MinAssign> seg(a);

seg.update(0, 2, 0LL); // assign 0 to [0..2]
seg.query(0, 4);        // 0
```

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <climits>

struct MinAssign {
	using S = int64_t;
	using F = int64_t;
	static constexpr F NONE = LLONG_MAX;
	static S op(S a, S b) { return std::min(a, b); }
	static S e() { return (int64_t)1e18; }
	static S mapping(F f, S x) { return f == NONE ? x : f; }
	static F composition(F f, F g) { return f == NONE ? g : f; }
	static F id() { return NONE; }
};

```
