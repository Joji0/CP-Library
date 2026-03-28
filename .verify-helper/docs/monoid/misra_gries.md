---
title: Misra-Gries Monoid
documentation_of: //monoid/misra_gries.hpp
---

Misra-Gries heavy hitters monoid. Maintains at most $K$ candidate elements that appear more than $n/(K+1)$ times. Supports segment tree queries to find frequent elements in a range.

- `MisraGriesMonoid<K>`: fixed $K$ candidates
- `Node(int val)`: single-element node
- `op(a, b)`: merge two summaries
