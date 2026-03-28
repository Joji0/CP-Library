---
title: Segment Tree
documentation_of: //ds/segtree/segtree.hpp
---

Point update / range query segment tree on a monoid.

### Usage

- `SegTree<Monoid>(int n)`: initialize with $n$ elements
- `SegTree<Monoid>(vector<T> A)`: build from array $A$
- `update(pos, val)`: $a_{pos} \gets val$
- `query(l, r)`: $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$
- `get(pos)`: $a_{pos}$
- `max_right(l, pred)`: largest $r$ satisfying `pred`
- `min_left(r, pred)`: smallest $l$ satisfying `pred`
- `find_first(l, r, check)`: leftmost position satisfying `check`
- `find_last(l, r, check)`: rightmost position satisfying `check`
