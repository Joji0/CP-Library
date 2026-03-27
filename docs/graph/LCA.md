---
title: LCA
documentation_of: graph/LCA.hpp
---

# LCA (Lowest Common Ancestor)

## Overview

Computes the **Lowest Common Ancestor** of two nodes in a rooted tree using [binary lifting](https://en.wikipedia.org/wiki/Level_ancestor_problem#Jump_pointer_algorithm).

Given a rooted tree with $n$ nodes, supports:

- **LCA query**: Find $\mathrm{lca}(a, b)$ in $O(\log n)$
- **$k$-th ancestor**: Lift a node $k$ levels up in $O(\log n)$
- **Distance**: Compute the number of edges between two nodes in $O(\log n)$
- **Subtree size**: Access `sub[v]` in $O(1)$ after building

Preprocessing takes $O(n \log n)$ time and space.

## Constructor

### `LCA(int n = 0)`

Creates a tree structure with $n$ nodes labeled $0, 1, \ldots, n-1$. No edges are added yet.

**Constraints**: $0 \le n$

**Complexity**: $O(n \log n)$

## Methods

### `void add_edge(int u, int v)`

Adds an undirected edge between nodes $u$ and $v$.

**Constraints**: $0 \le u, v < n$

**Complexity**: $O(1)$

### `void build(int root = 0)`

Roots the tree at `root` and precomputes all binary lifting tables, depths, and subtree sizes via DFS.

Must be called after all edges have been added and before any queries.

**Constraints**: All $n - 1$ edges must be added before calling `build`.

**Complexity**: $O(n \log n)$

### `int lca(int a, int b)`

Returns the lowest common ancestor of nodes $a$ and $b$.

**Constraints**: $0 \le a, b < n$, `build` has been called.

**Complexity**: $O(\log n)$

### `int lift(int u, int k)`

Returns the $k$-th ancestor of node $u$ (the node $k$ levels above $u$).

**Constraints**: $0 \le u < n$, $0 \le k \le \mathrm{depth}[u]$

**Complexity**: $O(\log n)$

### `int dist(int a, int b)`

Returns the number of edges on the path between $a$ and $b$.

Computed as $\mathrm{depth}[a] + \mathrm{depth}[b] - 2 \cdot \mathrm{depth}[\mathrm{lca}(a, b)]$.

**Constraints**: $0 \le a, b < n$, `build` has been called.

**Complexity**: $O(\log n)$

## Members

| Member | Description |
|--------|-------------|
| `depth[v]` | Depth of node $v$ (root has depth $0$) |
| `sub[v]` | Subtree size of node $v$ |
| `G[v]` | Adjacency list of node $v$ |

## Usage Example

```cpp
#include "graph/LCA.hpp"

int main() {
    LCA tree(7);
    //       0
    //      / \
    //     1   2
    //    /|    \
    //   3 4    5
    //   |
    //   6
    tree.add_edge(0, 1);
    tree.add_edge(0, 2);
    tree.add_edge(1, 3);
    tree.add_edge(1, 4);
    tree.add_edge(2, 5);
    tree.add_edge(3, 6);
    tree.build(0);

    tree.lca(3, 4);   // 1
    tree.lca(6, 5);   // 0
    tree.lca(6, 4);   // 1
    tree.dist(6, 5);  // 4
    tree.lift(6, 2);  // 1
    tree.sub[1];       // 4 (nodes 1,3,4,6)
}
```

## Verified with

* **WJ** [Library Checker: lca](https://judge.yosupo.jp/problem/lca) &mdash; `LCA.test.cpp`

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <vector>

struct LCA {
        int n, LOG;
        std::vector<std::vector<int>> G;
        std::vector<std::vector<int>> up;
        std::vector<int> depth, sub;
        LCA(int n = 0) { init(n); }
        void init(int n) {
                this->n = n;
                LOG = 1;
                while ((1 << LOG) <= n) LOG++;
                G.assign(n, {});
                up.assign(n, std::vector<int>(LOG, 0));
                depth.assign(n, 0);
                sub.assign(n, 1);
        }
        void add_edge(int u, int v) {
                G[u].push_back(v);
                G[v].push_back(u);
        }
        void dfs(int node, int par) {
                up[node][0] = par;
                for (int j = 1; j < LOG; j++) {
                        up[node][j] = up[up[node][j - 1]][j - 1];
                }
                for (int &nxt : G[node]) {
                        if (nxt == par) continue;
                        depth[nxt] = depth[node] + 1;
                        dfs(nxt, node);
                        sub[node] += sub[nxt];
                }
        }
        void build(int root = 0) {
                depth[root] = 0;
                dfs(root, root);
        }
        int lift(int u, int k) const {
                for (int j = LOG - 1; j >= 0; j--) {
                        if (k & (1 << j)) u = up[u][j];
                }
                return u;
        }
        int lca(int a, int b) const {
                if (depth[a] < depth[b]) std::swap(a, b);
                a = lift(a, depth[a] - depth[b]);
                if (a == b) return a;
                for (int j = LOG - 1; j >= 0; j--) {
                        if (up[a][j] != up[b][j]) {
                                a = up[a][j];
                                b = up[b][j];
                        }
                }
                return up[a][0];
        }
        int dist(int a, int b) const {
                int c = lca(a, b);
                return depth[a] + depth[b] - 2 * depth[c];
        }
};
```
