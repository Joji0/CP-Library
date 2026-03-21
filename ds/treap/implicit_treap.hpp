#pragma once
#include <algorithm>
#include <cassert>
#include <chrono>
#include <random>
#include <vector>

// ActedMonoid requirement:
// using S = ...; (Node Type)
// using F = ...; (Lazy Tag Type)
// static S op(S a, S b);
// static S e();
// static S mapping(F f, S x);
// static F composition(F f, F g); (f applied after g)
// static F id();

template <typename ActedMonoid> struct ImplicitTreap {
        using S = typename ActedMonoid::S;
        using F = typename ActedMonoid::F;
        struct Node {
                int l = 0, r = 0;
                int prior;
                int cnt = 1;
                bool rev = false;
                S val;
                S sum;
                F lazy;
                Node() = default;
                Node(S val, int prior) : prior(prior), val(val), sum(val), lazy(ActedMonoid::id()) {}
        };
        std::vector<Node> t;
        int root;
        std::mt19937 rng;
        ImplicitTreap() : root(0), rng(std::chrono::steady_clock::now().time_since_epoch().count()) {
                t.emplace_back();
        }
        ImplicitTreap(const std::vector<S> &A) : ImplicitTreap() { root = build(A, 0, (int)A.size() - 1); }
        int size() const { return cnt(root); }
        int cnt(int v) const { return v ? t[v].cnt : 0; }
        int new_node(S val) {
                int id = t.size();
                t.emplace_back(val, rng());
                return id;
        }
        void upd(int v) {
                if (!v) return;
                t[v].cnt = 1 + cnt(t[v].l) + cnt(t[v].r);
                S res = t[v].l ? t[t[v].l].sum : ActedMonoid::e();
                res = ActedMonoid::op(res, t[v].val);
                res = ActedMonoid::op(res, t[v].r ? t[t[v].r].sum : ActedMonoid::e());
                t[v].sum = res;
        }
        void apply(int v, const F &f) {
                if (!v) return;
                t[v].val = ActedMonoid::mapping(f, t[v].val);
                t[v].sum = ActedMonoid::mapping(f, t[v].sum);
                t[v].lazy = ActedMonoid::composition(f, t[v].lazy);
        }
        void push(int v) {
                if (!v) return;
                if (t[v].rev) {
                        std::swap(t[v].l, t[v].r);
                        if (t[v].l) t[t[v].l].rev ^= true;
                        if (t[v].r) t[t[v].r].rev ^= true;
                        t[v].rev = false;
                }
                apply(t[v].l, t[v].lazy);
                apply(t[v].r, t[v].lazy);
                t[v].lazy = ActedMonoid::id();
        }
        void split(int v, int k, int &l, int &r) {
                if (!v) {
                        l = r = 0;
                        return;
                }
                push(v);
                int cur_k = cnt(t[v].l) + 1;
                if (k >= cur_k) {
                        split(t[v].r, k - cur_k, t[v].r, r);
                        l = v;
                } else {
                        split(t[v].l, k, l, t[v].l);
                        r = v;
                }
                upd(v);
        }
        void merge(int &v, int l, int r) {
                if (!l || !r) {
                        v = l ? l : r;
                        return;
                }
                push(l);
                push(r);
                if (t[l].prior > t[r].prior) {
                        merge(t[l].r, t[l].r, r);
                        v = l;
                } else {
                        merge(t[r].l, l, t[r].l);
                        v = r;
                }
                upd(v);
        }
        int build(const std::vector<S> &A, int l, int r) {
                if (l > r) return 0;
                int mid = l + (r - l) / 2;
                int v = new_node(A[mid]);
                t[v].l = build(A, l, mid - 1);
                t[v].r = build(A, mid + 1, r);
                heapify(v);
                upd(v);
                return v;
        }
        void heapify(int v) {
                if (!v) return;
                int max_v = v;
                if (t[v].l && t[t[v].l].prior > t[max_v].prior) max_v = t[v].l;
                if (t[v].r && t[t[v].r].prior > t[max_v].prior) max_v = t[v].r;
                if (max_v != v) {
                        std::swap(t[v].prior, t[max_v].prior);
                        heapify(max_v);
                }
        }
        void insert(int pos, S val) {
                assert(0 <= pos && pos <= size());
                int t1, t2;
                split(root, pos, t1, t2);
                int v = new_node(val);
                merge(t1, t1, v);
                merge(root, t1, t2);
        }
        void erase(int pos) {
                assert(0 <= pos && pos < size());
                int t1, t2, t3;
                split(root, pos, t1, t2);
                split(t2, 1, t2, t3);
                merge(root, t1, t3);
        }
        void set(int pos, S new_val) {
                assert(0 <= pos && pos < size());
                int t1, t2, t3;
                split(root, pos, t1, t2);
                split(t2, 1, t2, t3);
                if (t2) {
                        t[t2].val = new_val;
                        upd(t2);
                }
                merge(t2, t1, t2);
                merge(root, t2, t3);
        }
        void update(int l, int r, const F &f) {
                if (l > r) return;
                assert(0 <= l && l <= r && r < size());
                int t1, t2, t3;
                split(root, r + 1, t2, t3);
                split(t2, l, t1, t2);
                apply(t2, f);
                merge(t2, t1, t2);
                merge(root, t2, t3);
        }
        void reverse(int l, int r) {
                if (l > r) return;
                assert(0 <= l && l <= r && r < size());
                int t1, t2, t3;
                split(root, r + 1, t2, t3);
                split(t2, l, t1, t2);
                if (t2) t[t2].rev ^= true;
                merge(t2, t1, t2);
                merge(root, t2, t3);
        }
        S query(int l, int r) {
                if (l > r) return ActedMonoid::e();
                assert(0 <= l && l <= r && r < size());
                int t1, t2, t3;
                split(root, r + 1, t2, t3);
                split(t2, l, t1, t2);
                S ans = t2 ? t[t2].sum : ActedMonoid::e();
                merge(t2, t1, t2);
                merge(root, t2, t3);
                return ans;
        }
        S get(int pos) { return query(pos, pos); }
};
