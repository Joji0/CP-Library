---
title: Segment Tree
documentation_of: ds/segtree/segtree.hpp
---

# Segment Tree

## Overview

A data structure for a [monoid](https://en.wikipedia.org/wiki/Monoid) $(S, \cdot, e)$ satisfying:

- **Associativity**: $(a \cdot b) \cdot c = a \cdot (b \cdot c)$ for all $a, b, c \in S$
- **Identity element**: $a \cdot e = e \cdot a = a$ for all $a \in S$

Given an array $a_0, a_1, \ldots, a_{n-1}$ of elements from $S$, the segment tree supports:

- **Point update**: Set $a_p \gets x$ in $O(\log n)$
- **Range query**: Compute $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$ in $O(\log n)$
- **Binary search on the tree**: `max_right`, `min_left`, `find_first`, `find_last` in $O(\log n)$

The implementation uses a **1-indexed recursive** approach with $4n$ node allocation.

## Template Parameter

The struct `SegTree` takes a **Monoid** type parameter that must provide:

| Member | Description |
|--------|-------------|
| `Monoid::value_type` | The element type $S$ |
| `Monoid::e()` | Returns the identity element $e$ |
| `Monoid::op(S a, S b)` | Returns $a \cdot b$ |

### Example Monoid Definitions

**Range Sum:**
```cpp
struct SumMonoid {
    using value_type = int64_t;
    static value_type e() { return 0; }
    static value_type op(value_type a, value_type b) { return a + b; }
};
```

**Range Max:**
```cpp
struct MaxMonoid {
    using value_type = int64_t;
    static value_type e() { return -1e18; }
    static value_type op(value_type a, value_type b) { return std::max(a, b); }
};
```

## Constructors

### `SegTree<Monoid>()`

Creates an empty segment tree with $n = 0$.

**Complexity**: $O(1)$

### `SegTree<Monoid>(int n)`

Creates a segment tree of $n$ elements, all initialized to `Monoid::e()`.

**Constraints**: $0 \le n$

**Complexity**: $O(n)$

### `SegTree<Monoid>(const std::vector<T> &A)`

Creates a segment tree from the vector $A$, where $n = \lvert A \rvert$.

**Constraints**: $0 \le \lvert A \rvert$

**Complexity**: $O(n)$

## Methods

### `void update(int pos, const T &new_val)`

Sets $a_{\mathrm{pos}} \gets \mathrm{new\_val}$.

**Constraints**: $0 \le \mathrm{pos} < n$

**Complexity**: $O(\log n)$

### `T query(int l, int r)`

Returns $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$.

Returns `Monoid::e()` if $l > r$.

**Constraints**: $0 \le l \le r < n$

**Complexity**: $O(\log n)$

### `T get(int pos)`

Returns $a_{\mathrm{pos}}$. Equivalent to `query(pos, pos)`.

**Constraints**: $0 \le \mathrm{pos} < n$

**Complexity**: $O(\log n)$

### `int max_right(int l, Pred pred)`

Returns the maximum $r$ such that `pred` applied to $a_l \cdot a_{l+1} \cdot \ldots \cdot a_{r-1}$ is `true`.

More formally, returns the maximum $r$ satisfying:

- $r \le n$
- $\mathrm{pred}(a_l \cdot a_{l+1} \cdot \ldots \cdot a_{r-1}) = \mathrm{true}$

If `pred` is monotone (i.e., once it becomes `false` it stays `false`), this is equivalent to finding the largest $r$ where the accumulated product still satisfies `pred`.

**Constraints**:
- $0 \le l \le n$
- $\mathrm{pred}(e()) = \mathrm{true}$

**Complexity**: $O(\log n)$

### `int min_left(int r, Pred pred)`

Returns the minimum $l$ such that `pred` applied to $a_l \cdot a_{l+1} \cdot \ldots \cdot a_{r-1}$ is `true`.

More formally, returns the minimum $l$ satisfying:

- $0 \le l$
- $\mathrm{pred}(a_l \cdot a_{l+1} \cdot \ldots \cdot a_{r-1}) = \mathrm{true}$

**Constraints**:
- $0 \le r \le n$
- $\mathrm{pred}(e()) = \mathrm{true}$

**Complexity**: $O(\log n)$

### `int find_first(int l, int r, Pred check)`

Returns the leftmost index $i \in [l, r]$ such that `check(a[i])` is `true` on some aggregate. Traverses left-to-right, returning the first node position where `check` is satisfied.

Returns $-1$ if no such index exists.

**Constraints**: $0 \le l \le r < n$

**Complexity**: $O(\log n)$

### `int find_last(int l, int r, Pred check)`

Returns the rightmost index $i \in [l, r]$ such that `check(a[i])` is `true` on some aggregate. Traverses right-to-left, returning the last node position where `check` is satisfied.

Returns $-1$ if no such index exists.

**Constraints**: $0 \le l \le r < n$

**Complexity**: $O(\log n)$

## Usage Example

```cpp
#include "ds/segtree/segtree.hpp"

struct MaxMonoid {
    using value_type = int;
    static int e() { return -1e9; }
    static int op(int a, int b) { return std::max(a, b); }
};

int main() {
    std::vector<int> a = {3, 1, 4, 1, 5, 9};
    SegTree<MaxMonoid> seg(a);

    seg.query(0, 3);      // max(3,1,4,1) = 4
    seg.update(1, 10);    // a = {3, 10, 4, 1, 5, 9}
    seg.query(0, 3);      // max(3,10,4,1) = 10

    // Find first r such that max of [0, r) > 7
    int r = seg.max_right(0, [](int x) { return x <= 7; }); // r = 1
}
```

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <cassert>
#include <vector>

template <typename Monoid> struct SegTree {
        using T = typename Monoid::value_type;
        int n;
        std::vector<T> t;
        SegTree() : n(0) {}
        SegTree(int n) : n(n) { t.resize(4 * n, Monoid::e()); }
        SegTree(const std::vector<T> &A) : n((int)A.size()) {
                t.resize(4 * n, Monoid::e());
                build(A, 1, 0, n - 1);
        }
        void build(const std::vector<T> &A, int v, int tl, int tr) {
                if (tl == tr) {
                        t[v] = A[tl];
                } else {
                        int tm = (tl + tr) / 2;
                        build(A, v * 2, tl, tm);
                        build(A, v * 2 + 1, tm + 1, tr);
                        t[v] = Monoid::op(t[v * 2], t[v * 2 + 1]);
                }
        }
        void update(int v, int tl, int tr, int pos, const T &new_val) {
                if (tl == tr) {
                        t[v] = new_val;
                } else {
                        int tm = (tl + tr) / 2;
                        if (pos <= tm) {
                                update(v * 2, tl, tm, pos, new_val);
                        } else {
                                update(v * 2 + 1, tm + 1, tr, pos, new_val);
                        }
                        t[v] = Monoid::op(t[v * 2], t[v * 2 + 1]);
                }
        }
        void update(int pos, const T &new_val) { update(1, 0, n - 1, pos, new_val); }
        T query(int v, int tl, int tr, int l, int r) const {
                if (l > r) {
                        return Monoid::e();
                }
                if (l == tl && r == tr) {
                        return t[v];
                }
                int tm = (tl + tr) / 2;
                return Monoid::op(query(v * 2, tl, tm, l, std::min(r, tm)),
                                  query(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r));
        }
        T query(int l, int r) const { return query(1, 0, n - 1, l, r); }
        T get(int pos) const { return query(pos, pos); }
        template <class Pred> int max_right(int l, Pred pred) const {
                assert(0 <= l && l <= n);
                assert(pred(Monoid::e()));
                T acc = Monoid::e();
                return max_right_dfs(1, 0, n - 1, l, pred, acc);
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
                int res = max_right_dfs(v * 2, tl, tm, l, pred, acc);
                if (res <= tm) return res;
                return max_right_dfs(v * 2 + 1, tm + 1, tr, l, pred, acc);
        }
        template <class Pred> int min_left(int r, Pred pred) const {
                assert(0 <= r && r <= n);
                assert(pred(Monoid::e()));
                T acc = Monoid::e();
                int res = min_left_dfs(1, 0, n - 1, r, pred, acc);
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
                int res = min_left_dfs(v * 2 + 1, tm + 1, tr, r, pred, acc);
                if (res >= tm + 1) return res;
                return min_left_dfs(v * 2, tl, tm, r, pred, acc);
        }
        template <class Pred> int find_first(int l, int r, Pred check) const {
                assert(0 <= l && l <= r && r < n);
                return find_first_dfs(1, 0, n - 1, l, r, check);
        }
        template <class Pred> int find_first_dfs(int v, int tl, int tr, int l, int r, Pred check) const {
                if (l > r || !check(t[v])) return -1;
                if (tl == tr) return tl;
                int tm = (tl + tr) / 2;
                int res = find_first_dfs(v * 2, tl, tm, l, std::min(r, tm), check);
                if (res != -1) return res;
                return find_first_dfs(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, check);
        }
        template <class Pred> int find_last(int l, int r, Pred check) const {
                assert(0 <= l && l <= r && r < n);
                return find_last_dfs(1, 0, n - 1, l, r, check);
        }
        template <class Pred> int find_last_dfs(int v, int tl, int tr, int l, int r, Pred check) const {
                if (l > r || !check(t[v])) return -1;
                if (tl == tr) return tl;
                int tm = (tl + tr) / 2;
                int res = find_last_dfs(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, check);
                if (res != -1) return res;
                return find_last_dfs(v * 2, tl, tm, l, std::min(r, tm), check);
        }
};
```
