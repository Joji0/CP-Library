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
