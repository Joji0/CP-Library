---
title: Implicit Treap
documentation_of: //ds/treap/implicit_treap.hpp
---

## Overview

An **implicit treap** (implicit key [Cartesian tree](https://en.wikipedia.org/wiki/Cartesian_tree)) that maintains a sequence with the following operations, all in $O(\log n)$ expected time:

- **Insert / Erase** at arbitrary position
- **Range update** with lazy propagation
- **Range query** on a monoid
- **Range reverse**
- **Point set / get**

The treap uses randomized priorities to maintain a balanced BST. Implicit keys (derived from subtree sizes) allow index-based access without storing explicit keys.

This is the most versatile sequence data structure — it supports everything a lazy segment tree does, plus insertions, deletions, and reversals.

## Template Parameter

Same as [Lazy Segment Tree](../segtree/lazy_segtree.md). The struct `ImplicitTreap` takes an **ActedMonoid** type parameter:

| Member | Description |
|--------|-------------|
| `ActedMonoid::S` | The element (node) type |
| `ActedMonoid::F` | The lazy tag (mapping) type |
| `ActedMonoid::op(S a, S b)` | Returns $a \cdot b$ |
| `ActedMonoid::e()` | Returns the identity element |
| `ActedMonoid::mapping(F f, S x)` | Returns $f(x)$ |
| `ActedMonoid::composition(F f, F g)` | Returns $f \circ g$ |
| `ActedMonoid::id()` | Returns the identity mapping |

### Example ActedMonoid

**Sum with Range Add:**
```cpp
struct AddSum {
    using S = int64_t;
    using F = int64_t;
    static S op(S a, S b) { return a + b; }
    static S e() { return 0; }
    static S mapping(F f, S x) { return x + f; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};
```

## Constructors

### `ImplicitTreap<ActedMonoid>()`

Creates an empty implicit treap.

**Complexity**: $O(1)$

### `ImplicitTreap<ActedMonoid>(const std::vector<S> &A)`

Creates an implicit treap from the vector $A$, building a balanced tree in $O(n)$.

**Complexity**: $O(n)$

## Methods

### `int size()`

Returns the number of elements in the sequence.

**Complexity**: $O(1)$

### `void insert(int pos, S val)`

Inserts element $val$ at position $pos$. Elements at positions $\ge pos$ are shifted right.

**Constraints**: $0 \le \mathrm{pos} \le \mathrm{size()}$

**Complexity**: $O(\log n)$ expected

### `void erase(int pos)`

Removes the element at position $pos$. Elements at positions $> pos$ are shifted left.

**Constraints**: $0 \le \mathrm{pos} < \mathrm{size()}$

**Complexity**: $O(\log n)$ expected

### `void set(int pos, S new_val)`

Sets the element at position $pos$ to $new\_val$.

**Constraints**: $0 \le \mathrm{pos} < \mathrm{size()}$

**Complexity**: $O(\log n)$ expected

### `void update(int l, int r, const F &f)`

Applies mapping $f$ to all elements in positions $[l, r]$, i.e., $a_i \gets f(a_i)$ for all $i \in [l, r]$.

**Constraints**: $0 \le l \le r < \mathrm{size()}$

**Complexity**: $O(\log n)$ expected

### `S query(int l, int r)`

Returns $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$.

Returns `ActedMonoid::e()` if $l > r$.

**Constraints**: $0 \le l \le r < \mathrm{size()}$

**Complexity**: $O(\log n)$ expected

### `S get(int pos)`

Returns $a_{\mathrm{pos}}$. Equivalent to `query(pos, pos)`.

**Constraints**: $0 \le \mathrm{pos} < \mathrm{size()}$

**Complexity**: $O(\log n)$ expected

### `void reverse(int l, int r)`

Reverses the subsequence $a_l, a_{l+1}, \ldots, a_r$ in-place.

**Constraints**: $0 \le l \le r < \mathrm{size()}$

**Complexity**: $O(\log n)$ expected

## Usage Example

```cpp
#include "ds/treap/implicit_treap.hpp"

struct AddMax {
    using S = int64_t;
    using F = int64_t;
    static S op(S a, S b) { return std::max(a, b); }
    static S e() { return -1e18; }
    static S mapping(F f, S x) { return x + f; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};

int main() {
    ImplicitTreap<AddMax> treap({3, 1, 4, 1, 5});

    treap.query(0, 4);      // max = 5
    treap.insert(2, 9);     // {3, 1, 9, 4, 1, 5}
    treap.query(0, 5);      // max = 9
    treap.erase(2);         // {3, 1, 4, 1, 5}
    treap.reverse(1, 3);    // {3, 1, 1, 4, 5}
    treap.update(0, 4, 10); // add 10: {13, 11, 11, 14, 15}
}
```
