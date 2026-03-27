---
title: Ordered Set
documentation_of: ds/ordered_set.hpp
---

# Ordered Set

## Overview

An **order-statistics tree** that maintains a set of **unique** elements with the following operations, all in $O(\log n)$:

- Standard set operations: insert, erase, membership test
- **$k$-th smallest element** (0-indexed)
- **Order of key**: number of elements strictly less than a given value
- **Range counting**: count elements in $[l, r]$ or $[l, r)$

Implemented as a wrapper around the [GNU Policy-Based Data Structure](https://gcc.gnu.org/onlinedocs/libstdc++/manual/policy_data_structures.html) (`__gnu_pbds::tree`), using a red-black tree with order-statistics node update.

**Note**: This is a **set** — duplicate elements are not allowed. For multiset behavior, see [OrderedMultiSet](./ordered_multiset.md).

## Template Parameters

| Parameter | Description | Default |
|-----------|-------------|---------|
| `T` | Element type | — |
| `Cmp` | Comparison function | `std::less<T>` |

## Constructor

### `OrderedSet<T, Cmp>()`

Creates an empty ordered set.

**Complexity**: $O(1)$

## Methods

### `int size()`

Returns the number of elements.

**Complexity**: $O(1)$

### `bool empty()`

Returns `true` if the set is empty.

**Complexity**: $O(1)$

### `void clear()`

Removes all elements.

**Complexity**: $O(n)$

### `bool insert(const T &x)`

Inserts element $x$. Returns `true` if $x$ was not already present, `false` otherwise.

**Complexity**: $O(\log n)$

### `bool erase(const T &x)`

Removes element $x$. Returns `true` if $x$ was present, `false` otherwise.

**Complexity**: $O(\log n)$

### `bool contains(const T &x)`

Returns `true` if $x$ is in the set.

**Complexity**: $O(\log n)$

### `int order_of_key(const T &x)`

Returns the number of elements **strictly less** than $x$. Equivalently, the 0-based index at which $x$ would appear.

**Complexity**: $O(\log n)$

### `optional<T> kth(int k)`

Returns the $k$-th smallest element (0-indexed). Returns `std::nullopt` if $k < 0$ or $k \ge \mathrm{size}()$.

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

Removes and returns the $k$-th smallest element (0-indexed). Returns `std::nullopt` if $k$ is out of range.

**Complexity**: $O(\log n)$

## Usage Example

```cpp
#include "ds/ordered_set.hpp"

int main() {
    OrderedSet<int> os;
    os.insert(10);
    os.insert(20);
    os.insert(30);
    os.insert(15);

    os.kth(0);            // 10
    os.kth(2);            // 20
    os.order_of_key(20);  // 2 (two elements < 20: 10, 15)
    os.count_range(10, 20); // 3 (10, 15, 20)

    os.erase_kth(1);      // removes 15, returns 15
    os.size();            // 3
}
```

## Source Code

```cpp
#pragma once

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <class T, class Cmp = less<T>> struct OrderedSet {
        using OST = tree<T, null_type, Cmp, rb_tree_tag, tree_order_statistics_node_update>;
        OST tr;
        int size() const { return (int)tr.size(); }
        bool empty() const { return tr.empty(); }
        void clear() { tr.clear(); }
        bool insert(const T &x) {
                auto [it, ok] = tr.insert(x);
                return ok;
        }
        bool erase(const T &x) { return tr.erase(x) > 0; }
        bool contains(const T &x) const { return tr.find(x) != tr.end(); }
        int order_of_key(const T &x) const { return (int)tr.order_of_key(x); }
        optional<T> kth(int k) const {
                if (k < 0 || k >= size()) return nullopt;
                auto it = tr.find_by_order(k);
                return *it;
        }
        optional<T> lower_bound(const T &x) const {
                auto it = tr.lower_bound(x);
                if (it == tr.end()) return nullopt;
                return *it;
        }
        optional<T> upper_bound(const T &x) const {
                auto it = tr.upper_bound(x);
                if (it == tr.end()) return nullopt;
                return *it;
        }
        int count_range(const T &l, const T &r) const {
                if (Cmp{}(r, l)) return 0;
                return order_of_key(r) - order_of_key(l) + (contains(r) ? 1 : 0);
        }
        int count_halfopen(const T &l, const T &r) const { return order_of_key(r) - order_of_key(l); }
        optional<T> erase_kth(int k) {
                if (k < 0 || k >= size()) return nullopt;
                auto it = tr.find_by_order(k);
                T val = *it;
                tr.erase(it);
                return val;
        }
};
```
