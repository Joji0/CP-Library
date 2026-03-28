---
title: Lazy Segment Tree
documentation_of: //ds/segtree/lazy_segtree.hpp
---

Range update / range query lazy segment tree on an acted monoid.

### Usage

- `LazySegTree<ActedMonoid>(int n)`: initialize with $n$ elements
- `LazySegTree<ActedMonoid>(vector<S> A)`: build from array $A$
- `update(l, r, f)`: for each $i \in [l,r]$, $a_i \gets f(a_i)$
- `update(pos, f)`: $a_{pos} \gets f(a_{pos})$
- `set(pos, val)`: $a_{pos} \gets val$
- `query(l, r)`: $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$
- `get(pos)`: $a_{pos}$
- `max_right(l, pred)`: largest $r$ satisfying `pred`
- `min_left(r, pred)`: smallest $l$ satisfying `pred`
