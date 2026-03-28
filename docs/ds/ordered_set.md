---
title: Ordered Set
documentation_of: //ds/ordered_set.hpp
---

Order statistics tree based on GNU PBDS. $k$-th element and rank queries in $O(\log n)$.

### Usage

- `OrderedSet<T>()`: initialize empty
- `insert(x)`: insert $x$
- `erase(x)`: erase $x$
- `order_of_key(x)`: number of elements less than $x$
- `kth(k)`: $k$-th element (0-indexed)
- `lower_bound(x)`, `upper_bound(x)`: iterators
