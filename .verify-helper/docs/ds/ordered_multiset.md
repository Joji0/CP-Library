---
title: Ordered MultiSet
documentation_of: //ds/ordered_multiset.hpp
---

## Overview

An **order-statistics multiset** that maintains a collection of elements (duplicates allowed) with all operations in $O(\log n)$:

- Standard multiset operations: insert, erase one/all occurrences, count
- **$k$-th smallest element** (0-indexed)
- **Order of key**: number of elements strictly less than a given value
- **Range counting**: count elements in $[l, r]$ or $[l, r)$
- **Min / Max** access

Built on top of GNU `__gnu_pbds::tree` using a **unique-ID trick**: each inserted element is stored as a pair `(value, unique_id)` to allow duplicate values while maintaining the unique-key invariant of the underlying tree.

## Template Parameters

| Parameter | Description | Default |
|-----------|-------------|---------|
| `T` | Element type | — |
| `Cmp` | Comparison function | `std::less<T>` |

## Constructor

### `OrderedMultiSet<T, Cmp>()`

Creates an empty ordered multiset.

**Complexity**: $O(1)$

## Methods

### `int size()`

Returns the total number of elements (counting duplicates).

**Complexity**: $O(1)$

### `bool empty()`

Returns `true` if the multiset is empty.

**Complexity**: $O(1)$

### `void clear()`

Removes all elements and resets the unique-ID counter.

**Complexity**: $O(n)$

### `void insert(const T &x)`

Inserts one copy of $x$.

**Complexity**: $O(\log n)$

### `bool erase_one(const T &x)`

Removes one occurrence of $x$. Returns `true` if $x$ was present, `false` otherwise.

**Complexity**: $O(\log n)$

### `int erase_all(const T &x)`

Removes all occurrences of $x$. Returns the number of elements removed.

**Complexity**: $O(k \log n)$ where $k$ is the number of occurrences

### `int count(const T &x)`

Returns the number of occurrences of $x$.

**Complexity**: $O(\log n)$

### `int order_of_key(const T &x)`

Returns the number of elements **strictly less** than $x$.

**Complexity**: $O(\log n)$

### `optional<T> kth(int k)`

Returns the $k$-th smallest element (0-indexed, counting duplicates). Returns `std::nullopt` if $k$ is out of range.

**Complexity**: $O(\log n)$

### `optional<T> lower_bound(const T &x)`

Returns the smallest element $\ge x$, or `std::nullopt` if no such element exists.

**Complexity**: $O(\log n)$

### `optional<T> upper_bound(const T &x)`

Returns the smallest element $> x$, or `std::nullopt` if no such element exists.

**Complexity**: $O(\log n)$

### `int count_range(const T &l, const T &r)`

Returns the number of elements in the closed interval $[l, r]$.

**Complexity**: $O(\log n)$

### `int count_halfopen(const T &l, const T &r)`

Returns the number of elements in the half-open interval $[l, r)$.

**Complexity**: $O(\log n)$

### `optional<T> erase_kth(int k)`

Removes and returns the $k$-th smallest element. Returns `std::nullopt` if $k$ is out of range.

**Complexity**: $O(\log n)$

### `optional<T> min_value()`

Returns the minimum element, or `std::nullopt` if empty.

**Complexity**: $O(\log n)$

### `optional<T> max_value()`

Returns the maximum element, or `std::nullopt` if empty.

**Complexity**: $O(\log n)$

## Usage Example

```cpp
#include "ds/ordered_multiset.hpp"

int main() {
    OrderedMultiSet<int> ms;
    ms.insert(3);
    ms.insert(1);
    ms.insert(3);
    ms.insert(2);

    ms.size();            // 4
    ms.kth(0);            // 1
    ms.kth(2);            // 3
    ms.count(3);          // 2
    ms.order_of_key(3);   // 2 (elements < 3: 1, 2)

    ms.erase_one(3);      // removes one 3
    ms.count(3);          // 1
    ms.min_value();       // 1
    ms.max_value();       // 3
}
```
