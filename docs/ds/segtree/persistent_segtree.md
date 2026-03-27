---
title: Persistent Segment Tree
documentation_of: ds/segtree/persistent_segtree.hpp
---

# Persistent Segment Tree

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

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <cassert>
#include <vector>

template <typename Monoid> struct PersistentSegTree {
        using T = typename Monoid::value_type;
        int n;
        std::vector<int> lc, rc;
        std::vector<T> t;
        std::vector<int> roots;
        PersistentSegTree(int n = 0) : n(n) {
                lc.push_back(0);
                rc.push_back(0);
                t.push_back(Monoid::e());
                if (n > 0) {
                        roots.push_back(build_empty(0, n - 1));
                }
        }
        PersistentSegTree(const std::vector<T> &A) : n((int)A.size()) {
                lc.push_back(0);
                rc.push_back(0);
                t.push_back(Monoid::e());
                roots.push_back(build(A, 0, n - 1));
        }
        int new_node(T val) {
                int id = t.size();
                lc.push_back(0);
                rc.push_back(0);
                t.push_back(val);
                return id;
        }
        int clone_node(int v) {
                int id = t.size();
                lc.push_back(lc[v]);
                rc.push_back(rc[v]);
                t.push_back(t[v]);
                return id;
        }
        int build_empty(int tl, int tr) {
                if (tl == tr) return new_node(Monoid::e());
                int tm = (tl + tr) / 2;
                int id = new_node(Monoid::e());
                lc[id] = build_empty(tl, tm);
                rc[id] = build_empty(tm + 1, tr);
                t[id] = Monoid::op(t[lc[id]], t[rc[id]]);
                return id;
        }
        int build(const std::vector<T> &A, int tl, int tr) {
                if (tl == tr) return new_node(A[tl]);
                int tm = (tl + tr) / 2;
                int id = new_node(Monoid::e());
                lc[id] = build(A, tl, tm);
                rc[id] = build(A, tm + 1, tr);
                t[id] = Monoid::op(t[lc[id]], t[rc[id]]);
                return id;
        }
        int update(int v, int tl, int tr, int pos, const T &new_val) {
                int id = clone_node(v);
                if (tl == tr) {
                        t[id] = new_val;
                        return id;
                }
                int tm = (tl + tr) / 2;
                if (pos <= tm) {
                        lc[id] = update(lc[id], tl, tm, pos, new_val);
                } else {
                        rc[id] = update(rc[id], tm + 1, tr, pos, new_val);
                }
                t[id] = Monoid::op(t[lc[id]], t[rc[id]]);
                return id;
        }
        int update(int prev_root, int pos, const T &new_val) {
                int new_root = update(prev_root, 0, n - 1, pos, new_val);
                roots.push_back(new_root);
                return new_root;
        }
        int update(int pos, const T &new_val) {
                assert(!roots.empty());
                return update(roots.back(), pos, new_val);
        }
        T query(int v, int tl, int tr, int l, int r) const {
                if (l > r) return Monoid::e();
                if (l == tl && r == tr) return t[v];
                int tm = (tl + tr) / 2;
                return Monoid::op(query(lc[v], tl, tm, l, std::min(r, tm)),
                                  query(rc[v], tm + 1, tr, std::max(l, tm + 1), r));
        }
        T query(int root, int l, int r) const {
                assert(0 <= l && l <= r && r < n);
                return query(root, 0, n - 1, l, r);
        }
        T get(int root, int pos) const { return query(root, pos, pos); }
        template <class Pred> int max_right(int root, int l, Pred pred) const {
                assert(0 <= l && l <= n);
                assert(pred(Monoid::e()));
                T acc = Monoid::e();
                return max_right_dfs(root, 0, n - 1, l, pred, acc);
        }
        template <class Pred> int max_right_dfs(int v, int tl, int tr, int l, Pred pred, T &acc) const {
                if (tr < l) return l;
                if (tl >= l) {
                        T nxt = Monoid::op(acc, t[v]);
                        if (pred(nxt)) {
                                acc = nxt;
                                return tr + 1;
                        }
                        if (tl == tr) return tl;
                }
                int tm = (tl + tr) / 2;
                int res = max_right_dfs(lc[v], tl, tm, l, pred, acc);
                if (res <= tm) return res;
                return max_right_dfs(rc[v], tm + 1, tr, l, pred, acc);
        }
        template <class Pred> int min_left(int root, int r, Pred pred) const {
                assert(0 <= r && r <= n);
                assert(pred(Monoid::e()));
                T acc = Monoid::e();
                int res = min_left_dfs(root, 0, n - 1, r, pred, acc);
                return res < 0 ? 0 : res;
        }
        template <class Pred> int min_left_dfs(int v, int tl, int tr, int r, Pred pred, T &acc) const {
                if (tl >= r) return r;
                if (tr < r) {
                        T nxt = Monoid::op(t[v], acc);
                        if (pred(nxt)) {
                                acc = nxt;
                                return tl - 1;
                        }
                        if (tl == tr) return tl + 1;
                }
                int tm = (tl + tr) / 2;
                int res = min_left_dfs(rc[v], tm + 1, tr, r, pred, acc);
                if (res >= tm + 1) return res;
                return min_left_dfs(lc[v], tl, tm, r, pred, acc);
        }
};
```
