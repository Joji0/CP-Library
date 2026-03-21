---
title: MaxAssign
documentation_of: acted_monoid/max_assign.hpp
---

# MaxAssign — Range Assign, Range Max

## Overview

ActedMonoid for lazy segment tree supporting **range assign** and **range max** queries.

Uses `LLONG_MIN` as a sentinel for "no assignment".

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `int64_t` | Max value |
| $F$ | `int64_t` | Assign value (`LLONG_MIN` = no-op) |
| $\mathrm{op}(a, b)$ | $\max(a, b)$ | Merge |
| $e$ | $-10^{18}$ | Identity |
| $\mathrm{mapping}(f, x)$ | $f = \mathrm{NONE}$ ? $x$ : $f$ | Assign |
| $\mathrm{composition}(f, g)$ | $f = \mathrm{NONE}$ ? $g$ : $f$ | Later wins |
| $\mathrm{id}$ | `LLONG_MIN` | No-op |

## Usage

```cpp
#include "acted_monoid/max_assign.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<int64_t> a = {3, 1, 4, 1, 5};
LazySegTree<MaxAssign> seg(a);

seg.update(0, 2, 10LL); // assign 10 to [0..2]
seg.query(0, 4);         // 10
```

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <climits>

struct MaxAssign {
	using S = int64_t;
	using F = int64_t;
	static constexpr F NONE = LLONG_MIN;
	static S op(S a, S b) { return std::max(a, b); }
	static S e() { return -(int64_t)1e18; }
	static S mapping(F f, S x) { return f == NONE ? x : f; }
	static F composition(F f, F g) { return f == NONE ? g : f; }
	static F id() { return NONE; }
};

```
