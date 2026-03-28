---
title: Ordered MultiSet
documentation_of: ds/ordered_multiset.hpp
---

Order statistics tree allowing duplicates. Internally assigns unique IDs.

### Usage

- `OrderedMultiSet<T>()`: initialize empty
- `insert(x)`: insert $x$
- `erase_one(x)`: erase one occurrence of $x$
- `erase_all(x)`: erase all occurrences of $x$
- `count(x)`: number of occurrences of $x$
- `order_of_key(x)`: number of elements less than $x$
- `kth(k)`: $k$-th element (0-indexed)
- `lower_bound(x)`, `upper_bound(x)`: iterators
