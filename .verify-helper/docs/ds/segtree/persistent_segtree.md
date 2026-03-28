---
title: Persistent Segment Tree
documentation_of: ds/segtree/persistent_segtree.hpp
---

Persistent segment tree with path copying for version management.

### Usage

- `PersistentSegTree<Monoid>(int n)`: initialize with $n$ elements
- `PersistentSegTree<Monoid>(vector<T> A)`: build from array $A$
- `update(prev_root, pos, val)`: return new root with $a_{pos} \gets val$ from a previous root
- `update(pos, val)`: $a_{pos} \gets val$ on the latest version
- `query(l, r)`: $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$
- `get(pos)`: $a_{pos}$
- `roots[t]`: root of version $t$
