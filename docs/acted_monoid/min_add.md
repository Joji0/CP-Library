---
title: MinAdd
documentation_of: acted_monoid/min_add.hpp
---

# MinAdd — Range Add, Range Min

## Overview

ActedMonoid for lazy segment tree supporting **range add** updates and **range min** queries.

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `int64_t` | Min value |
| $F$ | `int64_t` | Add value |
| $\mathrm{op}(a, b)$ | $\min(a, b)$ | Merge |
| $e$ | $10^{18}$ | Identity |
| $\mathrm{mapping}(f, x)$ | $x + f$ | Add $f$ to value |
| $\mathrm{composition}(f, g)$ | $f + g$ | Combine two adds |
| $\mathrm{id}$ | $0$ | No-op |

## Usage

```cpp
#include "acted_monoid/min_add.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<int64_t> a = {3, 1, 4, 1, 5};
LazySegTree<MinAdd> seg(a);

seg.query(0, 4);        // 1
seg.update(0, 4, 10LL); // add 10 to all
seg.query(0, 4);        // 11
```

## Source Code

```cpp
#pragma once
#include <algorithm>

struct MinAdd {
	using S = int64_t;
	using F = int64_t;
	static S op(S a, S b) { return std::min(a, b); }
	static S e() { return (int64_t)1e18; }
	static S mapping(F f, S x) { return x + f; }
	static F composition(F f, F g) { return f + g; }
	static F id() { return 0; }
};

```
