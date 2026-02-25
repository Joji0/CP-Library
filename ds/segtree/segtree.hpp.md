---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/3198.test.cpp
    title: test/2_yukicoder/3198.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/3227.test.cpp
    title: test/2_yukicoder/3227.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/3327.test.cpp
    title: test/2_yukicoder/3327.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/875.test.cpp
    title: test/2_yukicoder/875.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"ds/segtree/segtree.hpp\"\n#include <algorithm>\n#include\
    \ <cassert>\n#include <vector>\n\ntemplate <typename Monoid> struct SegTree {\n\
    \        using T = typename Monoid::value_type;\n        int n;\n        std::vector<T>\
    \ t;\n        SegTree() : n(0) {}\n        SegTree(int n) : n(n) { t.resize(4\
    \ * n, Monoid::e()); }\n        SegTree(const std::vector<T> &A) : n((int)A.size())\
    \ {\n                t.resize(4 * n, Monoid::e());\n                build(A, 1,\
    \ 0, n - 1);\n        }\n        void build(const std::vector<T> &A, int v, int\
    \ tl, int tr) {\n                if (tl == tr) {\n                        t[v]\
    \ = A[tl];\n                } else {\n                        int tm = (tl + tr)\
    \ / 2;\n                        build(A, v * 2, tl, tm);\n                   \
    \     build(A, v * 2 + 1, tm + 1, tr);\n                        t[v] = Monoid::op(t[v\
    \ * 2], t[v * 2 + 1]);\n                }\n        }\n        void update(int\
    \ v, int tl, int tr, int pos, const T &new_val) {\n                if (tl == tr)\
    \ {\n                        t[v] = new_val;\n                } else {\n     \
    \                   int tm = (tl + tr) / 2;\n                        if (pos <=\
    \ tm) {\n                                update(v * 2, tl, tm, pos, new_val);\n\
    \                        } else {\n                                update(v *\
    \ 2 + 1, tm + 1, tr, pos, new_val);\n                        }\n             \
    \           t[v] = Monoid::op(t[v * 2], t[v * 2 + 1]);\n                }\n  \
    \      }\n        void update(int pos, const T &new_val) { update(1, 0, n - 1,\
    \ pos, new_val); }\n        T query(int v, int tl, int tr, int l, int r) const\
    \ {\n                if (l > r) {\n                        return Monoid::e();\n\
    \                }\n                if (l == tl && r == tr) {\n              \
    \          return t[v];\n                }\n                int tm = (tl + tr)\
    \ / 2;\n                return Monoid::op(query(v * 2, tl, tm, l, std::min(r,\
    \ tm)),\n                                  query(v * 2 + 1, tm + 1, tr, std::max(l,\
    \ tm + 1), r));\n        }\n        T query(int l, int r) const { return query(1,\
    \ 0, n - 1, l, r); }\n        T get(int pos) const { return query(pos, pos); }\n\
    \        template <class Pred> int max_right(int l, Pred pred) const {\n     \
    \           assert(0 <= l && l <= n);\n                assert(pred(Monoid::e()));\n\
    \                T acc = Monoid::e();\n                return max_right_dfs(1,\
    \ 0, n - 1, l, pred, acc);\n        }\n        template <class Pred> int max_right_dfs(int\
    \ v, int tl, int tr, int l, Pred pred, T &acc) const {\n                if (tr\
    \ < l) return l;\n                if (tl >= l) {\n                        T nxt\
    \ = Monoid::op(acc, t[v]);\n                        if (pred(nxt)) {\n       \
    \                         acc = nxt;\n                                return tr\
    \ + 1;\n                        }\n                        if (tl == tr) return\
    \ tl;\n                }\n                int tm = (tl + tr) / 2;\n          \
    \      int res = max_right_dfs(v * 2, tl, tm, l, pred, acc);\n               \
    \ if (res <= tm) return res;\n                return max_right_dfs(v * 2 + 1,\
    \ tm + 1, tr, l, pred, acc);\n        }\n        template <class Pred> int min_left(int\
    \ r, Pred pred) const {\n                assert(0 <= r && r <= n);\n         \
    \       assert(pred(Monoid::e()));\n                T acc = Monoid::e();\n   \
    \             int res = min_left_dfs(1, 0, n - 1, r, pred, acc);\n           \
    \     return res < 0 ? 0 : res;\n        }\n        template <class Pred> int\
    \ min_left_dfs(int v, int tl, int tr, int r, Pred pred, T &acc) const {\n    \
    \            if (tl >= r) return r;\n                if (tr < r) {\n         \
    \               T nxt = Monoid::op(t[v], acc);\n                        if (pred(nxt))\
    \ {\n                                acc = nxt;\n                            \
    \    return tl - 1;\n                        }\n                        if (tl\
    \ == tr) return tl + 1;\n                }\n                int tm = (tl + tr)\
    \ / 2;\n                int res = min_left_dfs(v * 2 + 1, tm + 1, tr, r, pred,\
    \ acc);\n                if (res >= tm + 1) return res;\n                return\
    \ min_left_dfs(v * 2, tl, tm, r, pred, acc);\n        }\n        template <class\
    \ Pred> int find_first(int l, int r, Pred check) const {\n                assert(0\
    \ <= l && l <= r && r < n);\n                return find_first_dfs(1, 0, n - 1,\
    \ l, r, check);\n        }\n        template <class Pred> int find_first_dfs(int\
    \ v, int tl, int tr, int l, int r, Pred check) const {\n                if (l\
    \ > r || !check(t[v])) return -1;\n                if (tl == tr) return tl;\n\
    \                int tm = (tl + tr) / 2;\n                int res = find_first_dfs(v\
    \ * 2, tl, tm, l, std::min(r, tm), check);\n                if (res != -1) return\
    \ res;\n                return find_first_dfs(v * 2 + 1, tm + 1, tr, std::max(l,\
    \ tm + 1), r, check);\n        }\n        template <class Pred> int find_last(int\
    \ l, int r, Pred check) const {\n                assert(0 <= l && l <= r && r\
    \ < n);\n                return find_last_dfs(1, 0, n - 1, l, r, check);\n   \
    \     }\n        template <class Pred> int find_last_dfs(int v, int tl, int tr,\
    \ int l, int r, Pred check) const {\n                if (l > r || !check(t[v]))\
    \ return -1;\n                if (tl == tr) return tl;\n                int tm\
    \ = (tl + tr) / 2;\n                int res = find_last_dfs(v * 2 + 1, tm + 1,\
    \ tr, std::max(l, tm + 1), r, check);\n                if (res != -1) return res;\n\
    \                return find_last_dfs(v * 2, tl, tm, l, std::min(r, tm), check);\n\
    \        }\n};\n"
  code: "#pragma once\n#include <algorithm>\n#include <cassert>\n#include <vector>\n\
    \ntemplate <typename Monoid> struct SegTree {\n        using T = typename Monoid::value_type;\n\
    \        int n;\n        std::vector<T> t;\n        SegTree() : n(0) {}\n    \
    \    SegTree(int n) : n(n) { t.resize(4 * n, Monoid::e()); }\n        SegTree(const\
    \ std::vector<T> &A) : n((int)A.size()) {\n                t.resize(4 * n, Monoid::e());\n\
    \                build(A, 1, 0, n - 1);\n        }\n        void build(const std::vector<T>\
    \ &A, int v, int tl, int tr) {\n                if (tl == tr) {\n            \
    \            t[v] = A[tl];\n                } else {\n                       \
    \ int tm = (tl + tr) / 2;\n                        build(A, v * 2, tl, tm);\n\
    \                        build(A, v * 2 + 1, tm + 1, tr);\n                  \
    \      t[v] = Monoid::op(t[v * 2], t[v * 2 + 1]);\n                }\n       \
    \ }\n        void update(int v, int tl, int tr, int pos, const T &new_val) {\n\
    \                if (tl == tr) {\n                        t[v] = new_val;\n  \
    \              } else {\n                        int tm = (tl + tr) / 2;\n   \
    \                     if (pos <= tm) {\n                                update(v\
    \ * 2, tl, tm, pos, new_val);\n                        } else {\n            \
    \                    update(v * 2 + 1, tm + 1, tr, pos, new_val);\n          \
    \              }\n                        t[v] = Monoid::op(t[v * 2], t[v * 2\
    \ + 1]);\n                }\n        }\n        void update(int pos, const T &new_val)\
    \ { update(1, 0, n - 1, pos, new_val); }\n        T query(int v, int tl, int tr,\
    \ int l, int r) const {\n                if (l > r) {\n                      \
    \  return Monoid::e();\n                }\n                if (l == tl && r ==\
    \ tr) {\n                        return t[v];\n                }\n           \
    \     int tm = (tl + tr) / 2;\n                return Monoid::op(query(v * 2,\
    \ tl, tm, l, std::min(r, tm)),\n                                  query(v * 2\
    \ + 1, tm + 1, tr, std::max(l, tm + 1), r));\n        }\n        T query(int l,\
    \ int r) const { return query(1, 0, n - 1, l, r); }\n        T get(int pos) const\
    \ { return query(pos, pos); }\n        template <class Pred> int max_right(int\
    \ l, Pred pred) const {\n                assert(0 <= l && l <= n);\n         \
    \       assert(pred(Monoid::e()));\n                T acc = Monoid::e();\n   \
    \             return max_right_dfs(1, 0, n - 1, l, pred, acc);\n        }\n  \
    \      template <class Pred> int max_right_dfs(int v, int tl, int tr, int l, Pred\
    \ pred, T &acc) const {\n                if (tr < l) return l;\n             \
    \   if (tl >= l) {\n                        T nxt = Monoid::op(acc, t[v]);\n \
    \                       if (pred(nxt)) {\n                                acc\
    \ = nxt;\n                                return tr + 1;\n                   \
    \     }\n                        if (tl == tr) return tl;\n                }\n\
    \                int tm = (tl + tr) / 2;\n                int res = max_right_dfs(v\
    \ * 2, tl, tm, l, pred, acc);\n                if (res <= tm) return res;\n  \
    \              return max_right_dfs(v * 2 + 1, tm + 1, tr, l, pred, acc);\n  \
    \      }\n        template <class Pred> int min_left(int r, Pred pred) const {\n\
    \                assert(0 <= r && r <= n);\n                assert(pred(Monoid::e()));\n\
    \                T acc = Monoid::e();\n                int res = min_left_dfs(1,\
    \ 0, n - 1, r, pred, acc);\n                return res < 0 ? 0 : res;\n      \
    \  }\n        template <class Pred> int min_left_dfs(int v, int tl, int tr, int\
    \ r, Pred pred, T &acc) const {\n                if (tl >= r) return r;\n    \
    \            if (tr < r) {\n                        T nxt = Monoid::op(t[v], acc);\n\
    \                        if (pred(nxt)) {\n                                acc\
    \ = nxt;\n                                return tl - 1;\n                   \
    \     }\n                        if (tl == tr) return tl + 1;\n              \
    \  }\n                int tm = (tl + tr) / 2;\n                int res = min_left_dfs(v\
    \ * 2 + 1, tm + 1, tr, r, pred, acc);\n                if (res >= tm + 1) return\
    \ res;\n                return min_left_dfs(v * 2, tl, tm, r, pred, acc);\n  \
    \      }\n        template <class Pred> int find_first(int l, int r, Pred check)\
    \ const {\n                assert(0 <= l && l <= r && r < n);\n              \
    \  return find_first_dfs(1, 0, n - 1, l, r, check);\n        }\n        template\
    \ <class Pred> int find_first_dfs(int v, int tl, int tr, int l, int r, Pred check)\
    \ const {\n                if (l > r || !check(t[v])) return -1;\n           \
    \     if (tl == tr) return tl;\n                int tm = (tl + tr) / 2;\n    \
    \            int res = find_first_dfs(v * 2, tl, tm, l, std::min(r, tm), check);\n\
    \                if (res != -1) return res;\n                return find_first_dfs(v\
    \ * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, check);\n        }\n        template\
    \ <class Pred> int find_last(int l, int r, Pred check) const {\n             \
    \   assert(0 <= l && l <= r && r < n);\n                return find_last_dfs(1,\
    \ 0, n - 1, l, r, check);\n        }\n        template <class Pred> int find_last_dfs(int\
    \ v, int tl, int tr, int l, int r, Pred check) const {\n                if (l\
    \ > r || !check(t[v])) return -1;\n                if (tl == tr) return tl;\n\
    \                int tm = (tl + tr) / 2;\n                int res = find_last_dfs(v\
    \ * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, check);\n                if (res\
    \ != -1) return res;\n                return find_last_dfs(v * 2, tl, tm, l, std::min(r,\
    \ tm), check);\n        }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: ds/segtree/segtree.hpp
  requiredBy: []
  timestamp: '2026-01-21 11:02:21+07:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/2_yukicoder/875.test.cpp
  - test/2_yukicoder/3227.test.cpp
  - test/2_yukicoder/3327.test.cpp
  - test/2_yukicoder/3198.test.cpp
documentation_of: ds/segtree/segtree.hpp
layout: document
redirect_from:
- /library/ds/segtree/segtree.hpp
- /library/ds/segtree/segtree.hpp.html
title: ds/segtree/segtree.hpp
---
