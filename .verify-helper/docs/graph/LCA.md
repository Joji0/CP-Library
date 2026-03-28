---
title: LCA
documentation_of: graph/LCA.hpp
---

Lowest Common Ancestor via binary lifting. Preprocessing $O(n \log n)$, query $O(\log n)$.

### Usage

- `LCA(int n)`: initialize with $n$ vertices
- `add_edge(u, v)`: add edge $(u, v)$
- `build(root)`: build doubling table rooted at `root`
- `lca(u, v)`: LCA of $u$ and $v$
- `dist(u, v)`: distance (number of edges) between $u$ and $v$
- `lift(v, k)`: $k$-th ancestor of $v$
