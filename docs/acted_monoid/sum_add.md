---
title: SumAdd
documentation_of: acted_monoid/sum_add.hpp
---

# SumAdd — Range Add, Range Sum

## Overview

ActedMonoid for lazy segment tree supporting **range add** updates and **range sum** queries.

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `struct {long long val; int cnt;}` | Node stores sum and segment size |
| $F$ | `long long` | Add value |
| $\mathrm{op}(a, b)$ | `{a.val+b.val, a.cnt+b.cnt}` | Merge two nodes |
| $e$ | `{0, 0}` | Identity |
| $\mathrm{mapping}(f, x)$ | `{x.val + f*x.cnt, x.cnt}` | Add $f$ to each element |
| $\mathrm{composition}(f, g)$ | `f + g` | Combine two adds |
| $\mathrm{id}$ | `0` | No-op |

## Important

When constructing the segment tree, each leaf must have `cnt = 1`:

```cpp
std::vector<SumAdd::S> a;
for (int x : values) a.push_back({x, 1});
LazySegTree<SumAdd> seg(a);
```

## Usage

```cpp
#include "acted_monoid/sum_add.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<SumAdd::S> a;
for (int x : {3, 1, 4, 1, 5}) a.push_back({x, 1});
LazySegTree<SumAdd> seg(a);

seg.query(0, 4).val;    // 14 (sum of all)
seg.update(0, 2, 10LL); // add 10 to [0..2]
seg.query(0, 2).val;    // 38 (13+11+14)
```
