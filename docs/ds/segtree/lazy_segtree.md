---
title: Lazy Segment Tree
documentation_of: ds/segtree/lazy_segtree.hpp
---

# Lazy Segment Tree

## Overview

A data structure for a pair of a [monoid](https://en.wikipedia.org/wiki/Monoid) $(S, \cdot, e)$ and a set of mappings $F$ acting on $S$, satisfying:

- $F$ contains the identity mapping $\mathrm{id}$, where $\mathrm{id}(x) = x$ for all $x \in S$
- $F$ is closed under composition: for all $f, g \in F$, $f \circ g \in F$
- $f(x \cdot y) = f(x) \cdot f(y)$ holds for all $f \in F$ and $x, y \in S$ (endomorphism)

Given an array $a_0, a_1, \ldots, a_{n-1}$ of elements from $S$, the lazy segment tree supports:

- **Range update**: Apply $f \in F$ to all elements in $[l, r]$ in $O(\log n)$
- **Range query**: Compute $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$ in $O(\log n)$
- **Point set**: Set $a_p \gets x$ in $O(\log n)$
- **Binary search**: `max_right`, `min_left` in $O(\log n)$

## Template Parameter

The struct `LazySegTree` takes an **ActedMonoid** type parameter that must provide:

| Member | Description |
|--------|-------------|
| `ActedMonoid::S` | The element (node) type |
| `ActedMonoid::F` | The lazy tag (mapping) type |
| `ActedMonoid::op(S a, S b)` | Returns $a \cdot b$ |
| `ActedMonoid::e()` | Returns the identity element $e \in S$ |
| `ActedMonoid::mapping(F f, S x)` | Returns $f(x)$ |
| `ActedMonoid::composition(F f, F g)` | Returns $f \circ g$ (f applied **after** g) |
| `ActedMonoid::id()` | Returns the identity mapping $\mathrm{id} \in F$ |

### Example ActedMonoid Definitions

**Range Add, Range Sum:**
```cpp
struct AddSum {
    using S = int64_t;
    using F = int64_t;
    static S op(S a, S b) { return a + b; }
    static S e() { return 0; }
    static S mapping(F f, S x) { return x + f; }  // caution: needs size tracking for sums
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};
```

**Range Add, Range Max:**
```cpp
struct AddMax {
    using S = int64_t;
    using F = int64_t;
    static S op(S a, S b) { return std::max(a, b); }
    static S e() { return -1e18; }
    static S mapping(F f, S x) { return x + f; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};
```

**Range Assign, Range Min:**
```cpp
struct AssignMin {
    using S = int64_t;
    using F = int64_t;
    static constexpr F NONE = -1e18;
    static S op(S a, S b) { return std::min(a, b); }
    static S e() { return 1e18; }
    static S mapping(F f, S x) { return f == NONE ? x : f; }
    static F composition(F f, F g) { return f == NONE ? g : f; }
    static F id() { return NONE; }
};
```

## Constructors

### `LazySegTree<ActedMonoid>()`

Creates an empty lazy segment tree with $n = 0$.

**Complexity**: $O(1)$

### `LazySegTree<ActedMonoid>(int n)`

Creates a lazy segment tree of $n$ elements, all initialized to `ActedMonoid::e()` with lazy tags set to `ActedMonoid::id()`.

**Constraints**: $0 \le n$

**Complexity**: $O(n)$

### `LazySegTree<ActedMonoid>(const std::vector<S> &A)`

Creates a lazy segment tree from the vector $A$.

**Constraints**: $0 \le \lvert A \rvert$

**Complexity**: $O(n)$

## Methods

### `void update(int l, int r, const F &f)`

Applies mapping $f$ to all elements $a_l, a_{l+1}, \ldots, a_r$, i.e., $a_i \gets f(a_i)$ for all $i \in [l, r]$.

**Constraints**: $0 \le l \le r < n$

**Complexity**: $O(\log n)$

### `void update(int pos, const F &f)`

Applies mapping $f$ to $a_{\mathrm{pos}}$. Equivalent to `update(pos, pos, f)`.

**Constraints**: $0 \le \mathrm{pos} < n$

**Complexity**: $O(\log n)$

### `void set(int pos, const S &new_val)`

Sets $a_{\mathrm{pos}} \gets \mathrm{new\_val}$, clearing any pending lazy tag.

**Constraints**: $0 \le \mathrm{pos} < n$

**Complexity**: $O(\log n)$

### `S query(int l, int r)`

Returns $a_l \cdot a_{l+1} \cdot \ldots \cdot a_r$.

Returns `ActedMonoid::e()` if $l > r$.

**Constraints**: $0 \le l \le r < n$

**Complexity**: $O(\log n)$

### `S get(int pos)`

Returns $a_{\mathrm{pos}}$. Equivalent to `query(pos, pos)`.

**Constraints**: $0 \le \mathrm{pos} < n$

**Complexity**: $O(\log n)$

### `int max_right(int l, Pred pred)`

Returns the maximum $r$ such that `pred` applied to $a_l \cdot a_{l+1} \cdot \ldots \cdot a_{r-1}$ is `true`.

**Constraints**:
- $0 \le l \le n$
- $\mathrm{pred}(e()) = \mathrm{true}$

**Complexity**: $O(\log n)$

### `int min_left(int r, Pred pred)`

Returns the minimum $l$ such that `pred` applied to $a_l \cdot a_{l+1} \cdot \ldots \cdot a_{r-1}$ is `true`.

**Constraints**:
- $0 \le r \le n$
- $\mathrm{pred}(e()) = \mathrm{true}$

**Complexity**: $O(\log n)$

## Usage Example

```cpp
#include "ds/segtree/lazy_segtree.hpp"

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
    std::vector<int64_t> a = {3, 1, 4, 1, 5};
    LazySegTree<AddMax> seg(a);

    seg.query(0, 4);       // max(3,1,4,1,5) = 5
    seg.update(1, 3, 10);  // a = {3, 11, 14, 11, 5}
    seg.query(0, 4);       // max = 14
    seg.set(2, 0);         // a = {3, 11, 0, 11, 5}
}
```

## Source Code

```cpp
#pragma once
#include <algorithm>
#include <cassert>
#include <vector>

// ActedMonoid requirement:
// using S = ...; (Node Type)
// using F = ...; (Lazy Tag Type)
// static S op(S a, S b);
// static S e();
// static S mapping(F f, S x);
// static F composition(F f, F g); (f applied after g)
// static F id();

template <typename ActedMonoid> struct LazySegTree {
        using S = typename ActedMonoid::S;
        using F = typename ActedMonoid::F;
        int n;
        std::vector<S> t;
        std::vector<F> lazy;
        LazySegTree() : n(0) {}
        LazySegTree(int n) : n(n) {
                t.resize(4 * n, ActedMonoid::e());
                lazy.resize(4 * n, ActedMonoid::id());
        }
        LazySegTree(const std::vector<S> &A) : n((int)A.size()) {
                t.resize(4 * n, ActedMonoid::e());
                lazy.resize(4 * n, ActedMonoid::id());
                build(A, 1, 0, n - 1);
        }
        void build(const std::vector<S> &A, int v, int tl, int tr) {
                if (tl == tr) {
                        t[v] = A[tl];
                } else {
                        int tm = (tl + tr) / 2;
                        build(A, v * 2, tl, tm);
                        build(A, v * 2 + 1, tm + 1, tr);
                        t[v] = ActedMonoid::op(t[v * 2], t[v * 2 + 1]);
                }
        }
        void apply(int v, int tl, int tr, const F &f) {
                t[v] = ActedMonoid::mapping(f, t[v]);
                if (tl != tr) {
                        lazy[v] = ActedMonoid::composition(f, lazy[v]);
                }
        }
        void push(int v, int tl, int tr) {
                int tm = (tl + tr) / 2;
                apply(v * 2, tl, tm, lazy[v]);
                apply(v * 2 + 1, tm + 1, tr, lazy[v]);
                lazy[v] = ActedMonoid::id();
        }
        void update(int v, int tl, int tr, int l, int r, const F &f) {
                if (l > r) return;
                if (l == tl && r == tr) {
                        apply(v, tl, tr, f);
                } else {
                        push(v, tl, tr);
                        int tm = (tl + tr) / 2;
                        update(v * 2, tl, tm, l, std::min(r, tm), f);
                        update(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, f);
                        t[v] = ActedMonoid::op(t[v * 2], t[v * 2 + 1]);
                }
        }
        void update(int l, int r, const F &f) {
                assert(0 <= l && l <= r && r < n);
                update(1, 0, n - 1, l, r, f);
        }
        void update(int pos, const F &f) { update(pos, pos, f); }
        void set(int v, int tl, int tr, int pos, const S &new_val) {
                if (tl == tr) {
                        t[v] = new_val;
                        lazy[v] = ActedMonoid::id();
                } else {
                        push(v, tl, tr);
                        int tm = (tl + tr) / 2;
                        if (pos <= tm)
                                set(v * 2, tl, tm, pos, new_val);
                        else
                                set(v * 2 + 1, tm + 1, tr, pos, new_val);
                        t[v] = ActedMonoid::op(t[v * 2], t[v * 2 + 1]);
                }
        }
        void set(int pos, const S &new_val) { set(1, 0, n - 1, pos, new_val); }
        S query(int v, int tl, int tr, int l, int r) {
                if (l > r) return ActedMonoid::e();
                if (l == tl && r == tr) return t[v];
                push(v, tl, tr);
                int tm = (tl + tr) / 2;
                return ActedMonoid::op(query(v * 2, tl, tm, l, std::min(r, tm)),
                                       query(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r));
        }
        S query(int l, int r) {
                assert(0 <= l && l <= r && r < n);
                return query(1, 0, n - 1, l, r);
        }
        S get(int pos) { return query(pos, pos); }
        template <class Pred> int max_right(int l, Pred pred) {
                assert(0 <= l && l <= n);
                assert(pred(ActedMonoid::e()));
                S acc = ActedMonoid::e();
                return max_right_dfs(1, 0, n - 1, l, pred, acc);
        }
        template <class Pred> int max_right_dfs(int v, int tl, int tr, int l, Pred pred, S &acc) {
                if (tr < l) return l;
                if (tl >= l) {
                        S nxt = ActedMonoid::op(acc, t[v]);
                        if (pred(nxt)) {
                                acc = nxt;
                                return tr + 1;
                        }
                        if (tl == tr) return tl;
                }
                push(v, tl, tr);
                int tm = (tl + tr) / 2;
                int res = max_right_dfs(v * 2, tl, tm, l, pred, acc);
                if (res <= tm) return res;
                return max_right_dfs(v * 2 + 1, tm + 1, tr, l, pred, acc);
        }
        template <class Pred> int min_left(int r, Pred pred) {
                assert(0 <= r && r <= n);
                assert(pred(ActedMonoid::e()));
                S acc = ActedMonoid::e();
                int res = min_left_dfs(1, 0, n - 1, r, pred, acc);
                return res < 0 ? 0 : res;
        }
        template <class Pred> int min_left_dfs(int v, int tl, int tr, int r, Pred pred, S &acc) {
                if (tl >= r) return r;
                if (tr < r) {
                        S nxt = ActedMonoid::op(t[v], acc);
                        if (pred(nxt)) {
                                acc = nxt;
                                return tl - 1;
                        }
                        if (tl == tr) return tl + 1;
                }
                push(v, tl, tr);
                int tm = (tl + tr) / 2;
                int res = min_left_dfs(v * 2 + 1, tm + 1, tr, r, pred, acc);
                if (res >= tm + 1) return res;
                return min_left_dfs(v * 2, tl, tm, r, pred, acc);
        }
};
```
