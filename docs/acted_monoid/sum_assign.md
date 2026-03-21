---
title: SumAssign
documentation_of: acted_monoid/sum_assign.hpp
---

# SumAssign — Range Assign, Range Sum

## Overview

ActedMonoid for lazy segment tree supporting **range assign** (set all elements to a value) and **range sum** queries.

Uses `LLONG_MIN` as a sentinel value meaning "no assignment pending".

| Component | Type | Description |
|-----------|------|-------------|
| $S$ | `struct {long long val; int cnt;}` | Node stores sum and segment size |
| $F$ | `long long` | Assign value (`LLONG_MIN` = no-op) |
| $\mathrm{op}(a, b)$ | `{a.val+b.val, a.cnt+b.cnt}` | Merge two nodes |
| $e$ | `{0, 0}` | Identity |
| $\mathrm{mapping}(f, x)$ | $f = \mathrm{NONE}$ ? $x$ : $\{f \cdot x.\mathrm{cnt},\; x.\mathrm{cnt}\}$ | Assign $f$ to all |
| $\mathrm{composition}(f, g)$ | $f = \mathrm{NONE}$ ? $g$ : $f$ | Later assign wins |
| $\mathrm{id}$ | `LLONG_MIN` | No-op |

## Important

Leaves must have `cnt = 1`. Do **not** use `LLONG_MIN` as an actual data value.

## Usage

```cpp
#include "acted_monoid/sum_assign.hpp"
#include "ds/segtree/lazy_segtree.hpp"

std::vector<SumAssign::S> a;
for (int x : {3, 1, 4, 1, 5}) a.push_back({x, 1});
LazySegTree<SumAssign> seg(a);

seg.query(0, 4).val;    // 14
seg.update(0, 2, 7LL);  // assign 7 to [0..2]
seg.query(0, 2).val;    // 21 (7+7+7)
```
