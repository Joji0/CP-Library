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
  bundledCode: "#line 2 \"ds/unionfind/rollback_unionfind.hpp\"\n#include <algorithm>\n\
    #include <cassert>\n#include <vector>\n\nstruct RollbackDSU {\n        int n;\n\
    \        std::vector<int> psiz;\n        struct History {\n                int\
    \ u, v;\n                int size_u, parent_v;\n        };\n        std::vector<History>\
    \ history;\n        RollbackDSU() : n(0) {}\n        RollbackDSU(int n) : n(n)\
    \ { psiz.assign(n, -1); }\n        int find(int i) const {\n                while\
    \ (psiz[i] >= 0) {\n                        i = psiz[i];\n                }\n\
    \                return i;\n        }\n        bool join(int u, int v) {\n   \
    \             int root_u = find(u);\n                int root_v = find(v);\n \
    \               if (root_u == root_v) return false;\n                if (-psiz[root_u]\
    \ < -psiz[root_v]) {\n                        std::swap(root_u, root_v);\n   \
    \             }\n                history.push_back({root_u, root_v, psiz[root_u],\
    \ psiz[root_v]});\n                psiz[root_u] += psiz[root_v];\n           \
    \     psiz[root_v] = root_u;\n                return true;\n        }\n      \
    \  bool same(int u, int v) const { return find(u) == find(v); }\n        int size(int\
    \ i) const { return -psiz[find(i)]; }\n        int snapshot() const { return (int)history.size();\
    \ }\n        void rollback(int snap) {\n                while ((int)history.size()\
    \ > snap) {\n                        History h = history.back();\n           \
    \             history.pop_back();\n                        psiz[h.u] = h.size_u;\n\
    \                        psiz[h.v] = h.parent_v;\n                }\n        }\n\
    \        void undo() { rollback((int)history.size() - 1); }\n};\n"
  code: "#pragma once\n#include <algorithm>\n#include <cassert>\n#include <vector>\n\
    \nstruct RollbackDSU {\n        int n;\n        std::vector<int> psiz;\n     \
    \   struct History {\n                int u, v;\n                int size_u, parent_v;\n\
    \        };\n        std::vector<History> history;\n        RollbackDSU() : n(0)\
    \ {}\n        RollbackDSU(int n) : n(n) { psiz.assign(n, -1); }\n        int find(int\
    \ i) const {\n                while (psiz[i] >= 0) {\n                       \
    \ i = psiz[i];\n                }\n                return i;\n        }\n    \
    \    bool join(int u, int v) {\n                int root_u = find(u);\n      \
    \          int root_v = find(v);\n                if (root_u == root_v) return\
    \ false;\n                if (-psiz[root_u] < -psiz[root_v]) {\n             \
    \           std::swap(root_u, root_v);\n                }\n                history.push_back({root_u,\
    \ root_v, psiz[root_u], psiz[root_v]});\n                psiz[root_u] += psiz[root_v];\n\
    \                psiz[root_v] = root_u;\n                return true;\n      \
    \  }\n        bool same(int u, int v) const { return find(u) == find(v); }\n \
    \       int size(int i) const { return -psiz[find(i)]; }\n        int snapshot()\
    \ const { return (int)history.size(); }\n        void rollback(int snap) {\n \
    \               while ((int)history.size() > snap) {\n                       \
    \ History h = history.back();\n                        history.pop_back();\n \
    \                       psiz[h.u] = h.size_u;\n                        psiz[h.v]\
    \ = h.parent_v;\n                }\n        }\n        void undo() { rollback((int)history.size()\
    \ - 1); }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: ds/unionfind/rollback_unionfind.hpp
  requiredBy: []
  timestamp: '2026-02-25 14:14:54+07:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: ds/unionfind/rollback_unionfind.hpp
layout: document
redirect_from:
- /library/ds/unionfind/rollback_unionfind.hpp
- /library/ds/unionfind/rollback_unionfind.hpp.html
title: ds/unionfind/rollback_unionfind.hpp
---
