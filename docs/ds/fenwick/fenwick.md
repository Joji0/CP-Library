---
title: Fenwick Tree
documentation_of: //ds/fenwick/fenwick.hpp
---

Binary indexed tree. Point add and range sum in $O(\log n)$.

### Usage

- `Fenwick<T> fw(n)`: initialize length `n`
- `add(pos, delta)`: add `delta` to `a[pos]`
- `sum_prefix(r)`: sum on `[0, r]`
- `sum(l, r)`: sum on inclusive range `[l, r]`
- `lower_bound(x)`: first index with prefix sum at least `x`, assuming nonnegative values

`RangeFenwick<T>` supports range add and range sum.

### Usage

- `range_add(l, r, x)`: add `x` to inclusive range `[l, r]`
- `point_query(pos)`: value at `pos`
- `range_sum(l, r)`: sum on inclusive range `[l, r]`
