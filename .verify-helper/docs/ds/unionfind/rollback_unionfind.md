---
title: Rollback DSU
documentation_of: //ds/unionfind/rollback_unionfind.hpp
---

## Overview

A variant of [Union-Find](./unionfind.md) that supports **undo/rollback** operations. This is achieved by:

- Using **union by size** (no path compression, to keep operations reversible)
- Maintaining a **history stack** of all union operations

This allows restoring the data structure to any previous state, which is essential for algorithms like **offline dynamic connectivity** and **divide-and-conquer on queries**.

- **Union / Find**: $O(\log n)$ per operation (no path compression)
- **Rollback**: $O(k)$ where $k$ is the number of operations undone

## Constructor

### `RollbackDSU(int n)`

Creates $n$ singleton sets $\{0\}, \{1\}, \ldots, \{n-1\}$.

**Constraints**: $0 \le n$

**Complexity**: $O(n)$

## Methods

### `int find(int i)`

Returns the representative (root) of the set containing $i$. Does **not** perform path compression (to preserve rollback capability).

**Constraints**: $0 \le i < n$

**Complexity**: $O(\log n)$

### `bool join(int u, int v)`

Merges the sets containing $u$ and $v$. Returns `true` if a merge was performed, `false` if they were already in the same set. The operation is recorded in the history.

**Constraints**: $0 \le u, v < n$

**Complexity**: $O(\log n)$

### `bool same(int u, int v)`

Returns `true` if $u$ and $v$ belong to the same set.

**Constraints**: $0 \le u, v < n$

**Complexity**: $O(\log n)$

### `int size(int i)`

Returns the number of elements in the set containing $i$.

**Constraints**: $0 \le i < n$

**Complexity**: $O(\log n)$

### `int snapshot()`

Returns a snapshot identifier (the current size of the history stack). Use this value later in `rollback()` to restore the state at this point.

**Complexity**: $O(1)$

### `void rollback(int snap)`

Undoes all union operations performed after the snapshot `snap`, restoring the data structure to the state at that point.

**Constraints**: $0 \le \mathrm{snap} \le \mathrm{history.size()}$

**Complexity**: $O(k)$ where $k$ is the number of operations undone

### `void undo()`

Undoes the most recent union operation. Equivalent to `rollback(history.size() - 1)`.

**Constraints**: At least one union must have been performed.

**Complexity**: $O(1)$

## Usage Example

```cpp
#include "ds/unionfind/rollback_unionfind.hpp"

int main() {
    RollbackDSU dsu(5);

    dsu.join(0, 1);
    int snap = dsu.snapshot(); // save state: {0,1}, {2}, {3}, {4}

    dsu.join(2, 3);
    dsu.join(0, 2);
    dsu.same(0, 3); // true — {0,1,2,3}, {4}

    dsu.rollback(snap);
    dsu.same(0, 3); // false — back to {0,1}, {2}, {3}, {4}
}
```
