---
title: Persistent Segment Tree
documentation_of: //ds/segtree/persistent_segtree.hpp
---

## Overview

A **persistent** (also known as **functional**) segment tree for a monoid $(S, \cdot, e)$. Unlike a regular segment tree, every update creates a new version of the tree by path-copying, preserving all previous versions.

This allows:

- **Point update**: Set $a_p \gets x$, creating a new root (version) in $O(\log n)$
- **Range query on any version**: Compute $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$ on any historical version in $O(\log n)$
- **Binary search on any version**: `max_right`, `min_left` in $O(\log n)$

Each update allocates $O(\log n)$ new nodes via **path copying** (clone-on-write). Total memory for $q$ updates is $O(n + q \log n)$.

## Template Parameter

Same as [Segment Tree](./segtree.md). The struct `PersistentSegTree` takes a **Monoid** type parameter providing:

| Member | Description |
|--------|-------------|
| `Monoid::value_type` | The element type $S$ |
| `Monoid::e()` | Returns the identity element $e$ |
| `Monoid::op(S a, S b)` | Returns $a \cdot b$ |

## Constructors

### `PersistentSegTree<Monoid>(int n)`

Creates a persistent segment tree with $n$ elements initialized to `Monoid::e()`. One initial root (version 0) is created.

**Constraints**: $0 \le n$

**Complexity**: $O(n)$

### `PersistentSegTree<Monoid>(const std::vector<T> &A)`

Creates a persistent segment tree from the vector $A$. One initial root (version 0) is created.

**Constraints**: $0 \le \lvert A \rvert$

**Complexity**: $O(n)$

## Methods

### `int update(int prev_root, int pos, const T &new_val)`

Creates a new version where $a_{\mathrm{pos}} \gets \mathrm{new\_val}$, based on the version identified by `prev_root`. The new root is appended to the internal `roots` vector.

Returns the new root index.

**Constraints**: $0 \le \mathrm{pos} < n$

**Complexity**: $O(\log n)$ time and space

### `int update(int pos, const T &new_val)`

Creates a new version based on the **latest** version. Equivalent to `update(roots.back(), pos, new_val)`.

**Constraints**:
- $0 \le \mathrm{pos} < n$
- At least one version must exist

**Complexity**: $O(\log n)$ time and space

### `T query(int root, int l, int r)`

Returns $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$ in the version identified by `root`.

**Constraints**: $0 \le l \le r < n$

**Complexity**: $O(\log n)$

### `T get(int root, int pos)`

Returns $a_{\mathrm{pos}}$ in the version identified by `root`. Equivalent to `query(root, pos, pos)`.

**Constraints**: $0 \le \mathrm{pos} < n$

**Complexity**: $O(\log n)$

### `int max_right(int root, int l, Pred pred)`

On the version identified by `root`, returns the maximum $r$ such that `pred` applied to $a_l \cdot \ldots \cdot a_{r-1}$ is `true`.

**Constraints**:
- $0 \le l \le n$
- $\mathrm{pred}(e()) = \mathrm{true}$

**Complexity**: $O(\log n)$

### `int min_left(int root, int r, Pred pred)`

On the version identified by `root`, returns the minimum $l$ such that `pred` applied to $a_l \cdot \ldots \cdot a_{r-1}$ is `true`.

**Constraints**:
- $0 \le r \le n$
- $\mathrm{pred}(e()) = \mathrm{true}$

**Complexity**: $O(\log n)$

## Accessing Versions

The internal `roots` vector stores the root node index of each version:

- `roots[0]` — initial version (from constructor)
- `roots[k]` — version after the $k$-th update

Pass any `roots[k]` to `query`, `get`, `max_right`, or `min_left` to access that version.

## Usage Example

```cpp
#include "ds/segtree/persistent_segtree.hpp"

struct SumMonoid {
    using value_type = int64_t;
    static int64_t e() { return 0; }
    static int64_t op(int64_t a, int64_t b) { return a + b; }
};

int main() {
    std::vector<int64_t> a = {1, 2, 3, 4, 5};
    PersistentSegTree<SumMonoid> seg(a);

    // Version 0: [1, 2, 3, 4, 5]
    seg.query(seg.roots[0], 0, 4); // 15

    seg.update(2, 10);
    // Version 1: [1, 2, 10, 4, 5]
    seg.query(seg.roots[1], 0, 4); // 22

    // Version 0 is still accessible
    seg.query(seg.roots[0], 0, 4); // 15
}
```
