---
title: Union-Find (DSU)
documentation_of: ds/unionfind/unionfind.hpp
---

Union-Find with union by size and path compression.

### Usage

- `UnionFind(int n)`: initialize with $n$ vertices
- `find(x)`: root of $x$
- `join(x, y)`: merge $x$ and $y$, return new root
- `same(x, y)`: whether $x$ and $y$ are in the same component
- `size(x)`: size of the component containing $x$
- `groups()`: all connected components
- `num_comps`: number of components
