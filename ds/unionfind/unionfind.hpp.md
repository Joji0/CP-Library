---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/3200.test.cpp
    title: test/2_yukicoder/3200.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"ds/unionfind/unionfind.hpp\"\n#include <algorithm>\n#include\
    \ <numeric>\n#include <vector>\n\nstruct UnionFind {\n        std::vector<int>\
    \ parent, sz;\n        int num_comps;\n        UnionFind(int n = 0) { init(n);\
    \ }\n        void init(int n) {\n                parent.resize(n);\n         \
    \       iota(parent.begin(), parent.end(), 0);\n                sz.assign(n, 1);\n\
    \                num_comps = n;\n        }\n        int find(int x) { return (x\
    \ == parent[x] ? x : parent[x] = find(parent[x])); }\n        bool same(int a,\
    \ int b) { return find(a) == find(b); }\n        int size(int x) { return sz[find(x)];\
    \ }\n        bool join(int a, int b) {\n                a = find(a), b = find(b);\n\
    \                if (a != b) {\n                        if (sz[a] < sz[b]) std::swap(a,\
    \ b);\n                        parent[b] = a;\n                        sz[a] +=\
    \ sz[b];\n                        num_comps--;\n                        return\
    \ true;\n                }\n                return false;\n        }\n       \
    \ std::vector<std::vector<int>> groups() {\n                int n = parent.size();\n\
    \                std::vector<std::vector<int>> res(n);\n                std::vector<int>\
    \ group_size(n, 0);\n                for (int i = 0; i < n; i++) {\n         \
    \               group_size[find(i)]++;\n                }\n                std::vector<std::vector<int>>\
    \ result;\n                std::vector<int> root_map(n, -1);\n               \
    \ for (int i = 0; i < n; i++) {\n                        int r = find(i);\n  \
    \                      if (root_map[r] == -1) {\n                            \
    \    root_map[r] = result.size();\n                                result.push_back({});\n\
    \                                result.back().reserve(group_size[r]);\n     \
    \                   }\n                        result[root_map[r]].push_back(i);\n\
    \                }\n                return result;\n        }\n};\n"
  code: "#pragma once\n#include <algorithm>\n#include <numeric>\n#include <vector>\n\
    \nstruct UnionFind {\n        std::vector<int> parent, sz;\n        int num_comps;\n\
    \        UnionFind(int n = 0) { init(n); }\n        void init(int n) {\n     \
    \           parent.resize(n);\n                iota(parent.begin(), parent.end(),\
    \ 0);\n                sz.assign(n, 1);\n                num_comps = n;\n    \
    \    }\n        int find(int x) { return (x == parent[x] ? x : parent[x] = find(parent[x]));\
    \ }\n        bool same(int a, int b) { return find(a) == find(b); }\n        int\
    \ size(int x) { return sz[find(x)]; }\n        bool join(int a, int b) {\n   \
    \             a = find(a), b = find(b);\n                if (a != b) {\n     \
    \                   if (sz[a] < sz[b]) std::swap(a, b);\n                    \
    \    parent[b] = a;\n                        sz[a] += sz[b];\n               \
    \         num_comps--;\n                        return true;\n               \
    \ }\n                return false;\n        }\n        std::vector<std::vector<int>>\
    \ groups() {\n                int n = parent.size();\n                std::vector<std::vector<int>>\
    \ res(n);\n                std::vector<int> group_size(n, 0);\n              \
    \  for (int i = 0; i < n; i++) {\n                        group_size[find(i)]++;\n\
    \                }\n                std::vector<std::vector<int>> result;\n  \
    \              std::vector<int> root_map(n, -1);\n                for (int i =\
    \ 0; i < n; i++) {\n                        int r = find(i);\n               \
    \         if (root_map[r] == -1) {\n                                root_map[r]\
    \ = result.size();\n                                result.push_back({});\n  \
    \                              result.back().reserve(group_size[r]);\n       \
    \                 }\n                        result[root_map[r]].push_back(i);\n\
    \                }\n                return result;\n        }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: ds/unionfind/unionfind.hpp
  requiredBy: []
  timestamp: '2026-01-22 14:21:14+07:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/2_yukicoder/3200.test.cpp
documentation_of: ds/unionfind/unionfind.hpp
layout: document
redirect_from:
- /library/ds/unionfind/unionfind.hpp
- /library/ds/unionfind/unionfind.hpp.html
title: ds/unionfind/unionfind.hpp
---
