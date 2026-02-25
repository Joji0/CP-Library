---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"ds/segtree/lazy_segtree.hpp\"\n#include <algorithm>\n#include\
    \ <cassert>\n#include <vector>\n\n// ActedMonoid requirement:\n// using S = ...;\
    \ (Node Type)\n// using F = ...; (Lazy Tag Type)\n// static S op(S a, S b);\n\
    // static S e();\n// static S mapping(F f, S x);\n// static F composition(F f,\
    \ F g); (f applied after g)\n// static F id();\n\ntemplate <typename ActedMonoid>\
    \ struct LazySegTree {\n        using S = typename ActedMonoid::S;\n        using\
    \ F = typename ActedMonoid::F;\n        int n;\n        std::vector<S> t;\n  \
    \      std::vector<F> lazy;\n        LazySegTree() : n(0) {}\n        LazySegTree(int\
    \ n) : n(n) {\n                t.resize(4 * n, ActedMonoid::e());\n          \
    \      lazy.resize(4 * n, ActedMonoid::id());\n        }\n        LazySegTree(const\
    \ std::vector<S> &A) : n((int)A.size()) {\n                t.resize(4 * n, ActedMonoid::e());\n\
    \                lazy.resize(4 * n, ActedMonoid::id());\n                build(A,\
    \ 1, 0, n - 1);\n        }\n        void build(const std::vector<S> &A, int v,\
    \ int tl, int tr) {\n                if (tl == tr) {\n                       \
    \ t[v] = A[tl];\n                } else {\n                        int tm = (tl\
    \ + tr) / 2;\n                        build(A, v * 2, tl, tm);\n             \
    \           build(A, v * 2 + 1, tm + 1, tr);\n                        t[v] = ActedMonoid::op(t[v\
    \ * 2], t[v * 2 + 1]);\n                }\n        }\n        void apply(int v,\
    \ int tl, int tr, const F &f) {\n                t[v] = ActedMonoid::mapping(f,\
    \ t[v]);\n                if (tl != tr) {\n                        lazy[v] = ActedMonoid::composition(f,\
    \ lazy[v]);\n                }\n        }\n        void push(int v, int tl, int\
    \ tr) {\n                int tm = (tl + tr) / 2;\n                apply(v * 2,\
    \ tl, tm, lazy[v]);\n                apply(v * 2 + 1, tm + 1, tr, lazy[v]);\n\
    \                lazy[v] = ActedMonoid::id();\n        }\n        void update(int\
    \ v, int tl, int tr, int l, int r, const F &f) {\n                if (l > r) return;\n\
    \                if (l == tl && r == tr) {\n                        apply(v, tl,\
    \ tr, f);\n                } else {\n                        push(v, tl, tr);\n\
    \                        int tm = (tl + tr) / 2;\n                        update(v\
    \ * 2, tl, tm, l, std::min(r, tm), f);\n                        update(v * 2 +\
    \ 1, tm + 1, tr, std::max(l, tm + 1), r, f);\n                        t[v] = ActedMonoid::op(t[v\
    \ * 2], t[v * 2 + 1]);\n                }\n        }\n        void update(int\
    \ l, int r, const F &f) {\n                assert(0 <= l && l <= r && r < n);\n\
    \                update(1, 0, n - 1, l, r, f);\n        }\n        void update(int\
    \ pos, const F &f) { update(pos, pos, f); }\n        void set(int v, int tl, int\
    \ tr, int pos, const S &new_val) {\n                if (tl == tr) {\n        \
    \                t[v] = new_val;\n                        lazy[v] = ActedMonoid::id();\n\
    \                } else {\n                        push(v, tl, tr);\n        \
    \                int tm = (tl + tr) / 2;\n                        if (pos <= tm)\n\
    \                                set(v * 2, tl, tm, pos, new_val);\n         \
    \               else\n                                set(v * 2 + 1, tm + 1, tr,\
    \ pos, new_val);\n                        t[v] = ActedMonoid::op(t[v * 2], t[v\
    \ * 2 + 1]);\n                }\n        }\n        void set(int pos, const S\
    \ &new_val) { set(1, 0, n - 1, pos, new_val); }\n        S query(int v, int tl,\
    \ int tr, int l, int r) {\n                if (l > r) return ActedMonoid::e();\n\
    \                if (l == tl && r == tr) return t[v];\n                push(v,\
    \ tl, tr);\n                int tm = (tl + tr) / 2;\n                return ActedMonoid::op(query(v\
    \ * 2, tl, tm, l, std::min(r, tm)),\n                                       query(v\
    \ * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r));\n        }\n        S query(int\
    \ l, int r) {\n                assert(0 <= l && l <= r && r < n);\n          \
    \      return query(1, 0, n - 1, l, r);\n        }\n        S get(int pos) { return\
    \ query(pos, pos); }\n        template <class Pred> int max_right(int l, Pred\
    \ pred) {\n                assert(0 <= l && l <= n);\n                assert(pred(ActedMonoid::e()));\n\
    \                S acc = ActedMonoid::e();\n                return max_right_dfs(1,\
    \ 0, n - 1, l, pred, acc);\n        }\n        template <class Pred> int max_right_dfs(int\
    \ v, int tl, int tr, int l, Pred pred, S &acc) {\n                if (tr < l)\
    \ return l;\n                if (tl >= l) {\n                        S nxt = ActedMonoid::op(acc,\
    \ t[v]);\n                        if (pred(nxt)) {\n                         \
    \       acc = nxt;\n                                return tr + 1;\n         \
    \               }\n                        if (tl == tr) return tl;\n        \
    \        }\n                push(v, tl, tr);\n                int tm = (tl + tr)\
    \ / 2;\n                int res = max_right_dfs(v * 2, tl, tm, l, pred, acc);\n\
    \                if (res <= tm) return res;\n                return max_right_dfs(v\
    \ * 2 + 1, tm + 1, tr, l, pred, acc);\n        }\n        template <class Pred>\
    \ int min_left(int r, Pred pred) {\n                assert(0 <= r && r <= n);\n\
    \                assert(pred(ActedMonoid::e()));\n                S acc = ActedMonoid::e();\n\
    \                int res = min_left_dfs(1, 0, n - 1, r, pred, acc);\n        \
    \        return res < 0 ? 0 : res;\n        }\n        template <class Pred> int\
    \ min_left_dfs(int v, int tl, int tr, int r, Pred pred, S &acc) {\n          \
    \      if (tl >= r) return r;\n                if (tr < r) {\n               \
    \         S nxt = ActedMonoid::op(t[v], acc);\n                        if (pred(nxt))\
    \ {\n                                acc = nxt;\n                            \
    \    return tl - 1;\n                        }\n                        if (tl\
    \ == tr) return tl + 1;\n                }\n                push(v, tl, tr);\n\
    \                int tm = (tl + tr) / 2;\n                int res = min_left_dfs(v\
    \ * 2 + 1, tm + 1, tr, r, pred, acc);\n                if (res >= tm + 1) return\
    \ res;\n                return min_left_dfs(v * 2, tl, tm, r, pred, acc);\n  \
    \      }\n};\n"
  code: "#pragma once\n#include <algorithm>\n#include <cassert>\n#include <vector>\n\
    \n// ActedMonoid requirement:\n// using S = ...; (Node Type)\n// using F = ...;\
    \ (Lazy Tag Type)\n// static S op(S a, S b);\n// static S e();\n// static S mapping(F\
    \ f, S x);\n// static F composition(F f, F g); (f applied after g)\n// static\
    \ F id();\n\ntemplate <typename ActedMonoid> struct LazySegTree {\n        using\
    \ S = typename ActedMonoid::S;\n        using F = typename ActedMonoid::F;\n \
    \       int n;\n        std::vector<S> t;\n        std::vector<F> lazy;\n    \
    \    LazySegTree() : n(0) {}\n        LazySegTree(int n) : n(n) {\n          \
    \      t.resize(4 * n, ActedMonoid::e());\n                lazy.resize(4 * n,\
    \ ActedMonoid::id());\n        }\n        LazySegTree(const std::vector<S> &A)\
    \ : n((int)A.size()) {\n                t.resize(4 * n, ActedMonoid::e());\n \
    \               lazy.resize(4 * n, ActedMonoid::id());\n                build(A,\
    \ 1, 0, n - 1);\n        }\n        void build(const std::vector<S> &A, int v,\
    \ int tl, int tr) {\n                if (tl == tr) {\n                       \
    \ t[v] = A[tl];\n                } else {\n                        int tm = (tl\
    \ + tr) / 2;\n                        build(A, v * 2, tl, tm);\n             \
    \           build(A, v * 2 + 1, tm + 1, tr);\n                        t[v] = ActedMonoid::op(t[v\
    \ * 2], t[v * 2 + 1]);\n                }\n        }\n        void apply(int v,\
    \ int tl, int tr, const F &f) {\n                t[v] = ActedMonoid::mapping(f,\
    \ t[v]);\n                if (tl != tr) {\n                        lazy[v] = ActedMonoid::composition(f,\
    \ lazy[v]);\n                }\n        }\n        void push(int v, int tl, int\
    \ tr) {\n                int tm = (tl + tr) / 2;\n                apply(v * 2,\
    \ tl, tm, lazy[v]);\n                apply(v * 2 + 1, tm + 1, tr, lazy[v]);\n\
    \                lazy[v] = ActedMonoid::id();\n        }\n        void update(int\
    \ v, int tl, int tr, int l, int r, const F &f) {\n                if (l > r) return;\n\
    \                if (l == tl && r == tr) {\n                        apply(v, tl,\
    \ tr, f);\n                } else {\n                        push(v, tl, tr);\n\
    \                        int tm = (tl + tr) / 2;\n                        update(v\
    \ * 2, tl, tm, l, std::min(r, tm), f);\n                        update(v * 2 +\
    \ 1, tm + 1, tr, std::max(l, tm + 1), r, f);\n                        t[v] = ActedMonoid::op(t[v\
    \ * 2], t[v * 2 + 1]);\n                }\n        }\n        void update(int\
    \ l, int r, const F &f) {\n                assert(0 <= l && l <= r && r < n);\n\
    \                update(1, 0, n - 1, l, r, f);\n        }\n        void update(int\
    \ pos, const F &f) { update(pos, pos, f); }\n        void set(int v, int tl, int\
    \ tr, int pos, const S &new_val) {\n                if (tl == tr) {\n        \
    \                t[v] = new_val;\n                        lazy[v] = ActedMonoid::id();\n\
    \                } else {\n                        push(v, tl, tr);\n        \
    \                int tm = (tl + tr) / 2;\n                        if (pos <= tm)\n\
    \                                set(v * 2, tl, tm, pos, new_val);\n         \
    \               else\n                                set(v * 2 + 1, tm + 1, tr,\
    \ pos, new_val);\n                        t[v] = ActedMonoid::op(t[v * 2], t[v\
    \ * 2 + 1]);\n                }\n        }\n        void set(int pos, const S\
    \ &new_val) { set(1, 0, n - 1, pos, new_val); }\n        S query(int v, int tl,\
    \ int tr, int l, int r) {\n                if (l > r) return ActedMonoid::e();\n\
    \                if (l == tl && r == tr) return t[v];\n                push(v,\
    \ tl, tr);\n                int tm = (tl + tr) / 2;\n                return ActedMonoid::op(query(v\
    \ * 2, tl, tm, l, std::min(r, tm)),\n                                       query(v\
    \ * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r));\n        }\n        S query(int\
    \ l, int r) {\n                assert(0 <= l && l <= r && r < n);\n          \
    \      return query(1, 0, n - 1, l, r);\n        }\n        S get(int pos) { return\
    \ query(pos, pos); }\n        template <class Pred> int max_right(int l, Pred\
    \ pred) {\n                assert(0 <= l && l <= n);\n                assert(pred(ActedMonoid::e()));\n\
    \                S acc = ActedMonoid::e();\n                return max_right_dfs(1,\
    \ 0, n - 1, l, pred, acc);\n        }\n        template <class Pred> int max_right_dfs(int\
    \ v, int tl, int tr, int l, Pred pred, S &acc) {\n                if (tr < l)\
    \ return l;\n                if (tl >= l) {\n                        S nxt = ActedMonoid::op(acc,\
    \ t[v]);\n                        if (pred(nxt)) {\n                         \
    \       acc = nxt;\n                                return tr + 1;\n         \
    \               }\n                        if (tl == tr) return tl;\n        \
    \        }\n                push(v, tl, tr);\n                int tm = (tl + tr)\
    \ / 2;\n                int res = max_right_dfs(v * 2, tl, tm, l, pred, acc);\n\
    \                if (res <= tm) return res;\n                return max_right_dfs(v\
    \ * 2 + 1, tm + 1, tr, l, pred, acc);\n        }\n        template <class Pred>\
    \ int min_left(int r, Pred pred) {\n                assert(0 <= r && r <= n);\n\
    \                assert(pred(ActedMonoid::e()));\n                S acc = ActedMonoid::e();\n\
    \                int res = min_left_dfs(1, 0, n - 1, r, pred, acc);\n        \
    \        return res < 0 ? 0 : res;\n        }\n        template <class Pred> int\
    \ min_left_dfs(int v, int tl, int tr, int r, Pred pred, S &acc) {\n          \
    \      if (tl >= r) return r;\n                if (tr < r) {\n               \
    \         S nxt = ActedMonoid::op(t[v], acc);\n                        if (pred(nxt))\
    \ {\n                                acc = nxt;\n                            \
    \    return tl - 1;\n                        }\n                        if (tl\
    \ == tr) return tl + 1;\n                }\n                push(v, tl, tr);\n\
    \                int tm = (tl + tr) / 2;\n                int res = min_left_dfs(v\
    \ * 2 + 1, tm + 1, tr, r, pred, acc);\n                if (res >= tm + 1) return\
    \ res;\n                return min_left_dfs(v * 2, tl, tm, r, pred, acc);\n  \
    \      }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: ds/segtree/lazy_segtree.hpp
  requiredBy: []
  timestamp: '2026-02-25 14:14:54+07:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: ds/segtree/lazy_segtree.hpp
layout: document
redirect_from:
- /library/ds/segtree/lazy_segtree.hpp
- /library/ds/segtree/lazy_segtree.hpp.html
title: ds/segtree/lazy_segtree.hpp
---
