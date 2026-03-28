---
title: Sparse Table
documentation_of: //ds/sparse_table/sparse_table.hpp
---

Static range query on an idempotent monoid. Preprocessing $O(n \log n)$, query $O(1)$.

### Usage

- `SparseTable<Monoid>(vector<T> A)`: build from array $A$
- `query(l, r)`: $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$
