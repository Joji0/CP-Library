---
title: Union-Find (DSU)
documentation_of: ds/unionfind/unionfind.hpp
---

# Union-Find (Disjoint Set Union)

## Overview

A data structure that manages a partition of $\{0, 1, \ldots, n-1\}$ into disjoint sets. Supports:

- **Union**: Merge two sets in $O(\alpha(n))$ amortized
- **Find**: Determine which set an element belongs to in $O(\alpha(n))$ amortized
- **Component enumeration**: List all groups in $O(n)$

Here $\alpha$ denotes the [inverse Ackermann function](https://en.wikipedia.org/wiki/Ackermann_function#Inverse), which is effectively constant ($\le 4$) for all practical input sizes.

The implementation uses **path compression** and **union by size**.

## Constructor

### `UnionFind(int n = 0)`

Creates $n$ singleton sets $\{0\}, \{1\}, \ldots, \{n-1\}$.

**Constraints**: $0 \le n$

**Complexity**: $O(n)$

## Methods

### `int find(int x)`

Returns the representative (root) of the set containing $x$.

**Constraints**: $0 \le x < n$

**Complexity**: $O(\alpha(n))$ amortized

### `bool join(int a, int b)`

Merges the sets containing $a$ and $b$. Returns `true` if $a$ and $b$ were in different sets (i.e., a merge was performed), `false` otherwise.

**Constraints**: $0 \le a, b < n$

**Complexity**: $O(\alpha(n))$ amortized

### `bool same(int a, int b)`

Returns `true` if $a$ and $b$ belong to the same set.

**Constraints**: $0 \le a, b < n$

**Complexity**: $O(\alpha(n))$ amortized

### `int size(int x)`

Returns the number of elements in the set containing $x$.

**Constraints**: $0 \le x < n$

**Complexity**: $O(\alpha(n))$ amortized

### `std::vector<std::vector<int>> groups()`

Returns all connected components as a vector of vectors. Each inner vector contains the elements of one component in ascending order.

**Complexity**: $O(n \cdot \alpha(n))$

## Members

| Member | Description |
|--------|-------------|
| `num_comps` | The current number of disjoint sets. Decremented by 1 on each successful `join`. |

## Usage Example

```cpp
#include "ds/unionfind/unionfind.hpp"

int main() {
    UnionFind uf(5); // {0}, {1}, {2}, {3}, {4}
    uf.num_comps;    // 5

    uf.join(0, 1);   // true — {0,1}, {2}, {3}, {4}
    uf.join(2, 3);   // true — {0,1}, {2,3}, {4}
    uf.same(0, 1);   // true
    uf.same(0, 2);   // false
    uf.size(0);      // 2
    uf.num_comps;    // 3

    auto g = uf.groups(); // [[0,1], [2,3], [4]]
}
```

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <numeric>
#include <vector>

struct UnionFind {
        std::vector<int> parent, sz;
        int num_comps;
        UnionFind(int n = 0) { init(n); }
        void init(int n) {
                parent.resize(n);
                iota(parent.begin(), parent.end(), 0);
                sz.assign(n, 1);
                num_comps = n;
        }
        int find(int x) { return (x == parent[x] ? x : parent[x] = find(parent[x])); }
        bool same(int a, int b) { return find(a) == find(b); }
        int size(int x) { return sz[find(x)]; }
        bool join(int a, int b) {
                a = find(a), b = find(b);
                if (a != b) {
                        if (sz[a] < sz[b]) std::swap(a, b);
                        parent[b] = a;
                        sz[a] += sz[b];
                        num_comps--;
                        return true;
                }
                return false;
        }
        std::vector<std::vector<int>> groups() {
                int n = parent.size();
                std::vector<std::vector<int>> res(n);
                std::vector<int> group_size(n, 0);
                for (int i = 0; i < n; i++) {
                        group_size[find(i)]++;
                }
                std::vector<std::vector<int>> result;
                std::vector<int> root_map(n, -1);
                for (int i = 0; i < n; i++) {
                        int r = find(i);
                        if (root_map[r] == -1) {
                                root_map[r] = result.size();
                                result.push_back({});
                                result.back().reserve(group_size[r]);
                        }
                        result[root_map[r]].push_back(i);
                }
                return result;
        }
};

```
