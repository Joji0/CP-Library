---
title: Dynamic Misra-Gries Monoid
documentation_of: //monoid/dynamic_misra_gries.hpp
---

Misra-Gries heavy hitters with per-node dynamic $K$. `MAX_K` is the compile-time upper bound; each node stores its own $K$ value. Useful when different queries need different thresholds.

- `DynamicMisraGriesMonoid<MAX_K>`: max $K$ candidates
- `Node(int val, int k)`: single-element node with threshold $k$
- `op(a, b)`: merge using $\max(a.k, b.k)$
