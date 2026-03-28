---
title: Implicit Treap
documentation_of: ds/treap/implicit_treap.hpp
---

Implicit key treap on an acted monoid. Supports range reverse, insert, and erase in $O(\log n)$.

### Usage

- `ImplicitTreap<ActedMonoid>(int n)`: initialize with $n$ elements
- `ImplicitTreap<ActedMonoid>(vector<S> A)`: build from array $A$
- `insert(pos, val)`: insert $val$ at position $pos$
- `erase(pos)`: erase element at position $pos$
- `set(pos, val)`: $a_{pos} \gets val$
- `update(l, r, f)`: for each $i \in [l,r]$, $a_i \gets f(a_i)$
- `query(l, r)`: $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$
- `reverse(l, r)`: reverse the range $[l, r]$
