---
title: Sparse Table
documentation_of: //ds/sparse_table/sparse_table.hpp
---

Static range query on an idempotent monoid. Preprocessing $O(n \log n)$, query $O(1)$.

### Usage

- `SparseTable<Monoid> st(A)`: build from array `A`
- `init(A)`: rebuild from array `A`
- `query(l, r)`: product on inclusive range `[l, r]`

Example: `SparseTable<MinMonoid<int>> st(a)`.
