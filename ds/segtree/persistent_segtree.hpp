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
