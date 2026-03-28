---
title: Rollback DSU
documentation_of: //ds/unionfind/rollback_unionfind.hpp
---

Rollback-capable Union-Find. Union by size only (no path compression).

### Usage

- `RollbackUnionFind(int n)`: initialize with $n$ vertices
- `find(x)`: root of $x$
- `join(x, y)`: merge $x$ and $y$
- `same(x, y)`: whether $x$ and $y$ are in the same component
- `size(x)`: size of the component containing $x$
- `snapshot()`: save current state
- `rollback()`: restore to last snapshot
- `undo()`: undo the last join
