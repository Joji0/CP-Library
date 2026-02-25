---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: ds/hashmap.hpp
    title: ds/hashmap.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/associative_array
    links:
    - https://judge.yosupo.jp/problem/associative_array
  bundledCode: "#line 1 \"test/1_library_checker/ds/hashmap.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/associative_array\"\n#line 2 \"ds/hashmap.hpp\"\
    \n#include <cassert>\n#include <chrono>\n#include <cstdint>\n#include <vector>\n\
    \ntemplate <typename K, typename V> struct HashMap {\n      private:\n       \
    \ struct Node {\n                K key;\n                V val;\n            \
    \    bool exists = false;\n        };\n        std::vector<Node> table;\n    \
    \    uint32_t cap, size;\n        uint64_t random_seed;\n        static uint64_t\
    \ splitmix64(uint64_t x) {\n                // http://xorshift.di.unimi.it/splitmix64.c\n\
    \                x += 0x9e3779b97f4a7c15;\n                x = (x ^ (x >> 30))\
    \ * 0xbf58476d1ce4e5b9;\n                x = (x ^ (x >> 27)) * 0x94d049bb133111eb;\n\
    \                return x ^ (x >> 31);\n        }\n        uint32_t get_index(K\
    \ key) {\n                uint64_t hash = splitmix64((uint64_t)key + random_seed);\n\
    \                return hash & (cap - 1);\n        }\n        void expand() {\n\
    \                std::vector<Node> old_table = table;\n                build(cap\
    \ << 1);\n                for (const auto &node : old_table) {\n             \
    \           if (node.exists) {\n                                uint32_t idx =\
    \ get_index(node.key);\n                                while (table[idx].exists)\
    \ {\n                                        idx++;\n                        \
    \                idx &= (cap - 1);\n                                }\n      \
    \                          table[idx] = {node.key, node.val, true};\n        \
    \                        size++;\n                        }\n                }\n\
    \        }\n\n      public:\n        HashMap(uint32_t n = 1 << 20) {\n       \
    \         uint32_t pw = 1;\n                while (pw < n) pw <<= 1;\n       \
    \         build(pw);\n        }\n        void build(uint32_t n) {\n          \
    \      cap = n;\n                size = 0;\n                table.assign(cap,\
    \ {});\n                random_seed = std::chrono::steady_clock::now().time_since_epoch().count();\n\
    \        }\n        void insert(K key, V val) { (*this)[key] = val; }\n      \
    \  V &operator[](K key) {\n                if (size >= cap / 2) expand();\n  \
    \              uint32_t idx = get_index(key);\n                while (table[idx].exists)\
    \ {\n                        if (table[idx].key == key) {\n                  \
    \              return table[idx].val;\n                        }\n           \
    \             idx++;\n                        idx &= (cap - 1);\n            \
    \    }\n                table[idx] = {key, V{}, true};\n                size++;\n\
    \                return table[idx].val;\n        }\n        V get(K key, V default_val\
    \ = -1) {\n                uint32_t idx = get_index(key);\n                while\
    \ (table[idx].exists) {\n                        if (table[idx].key == key) {\n\
    \                                return table[idx].val;\n                    \
    \    }\n                        idx++;\n                        idx &= (cap -\
    \ 1);\n                }\n                return default_val;\n        }\n   \
    \     int sz() { return size; }\n};\n#line 3 \"test/1_library_checker/ds/hashmap.test.cpp\"\
    \n#include <bits/stdc++.h>\nusing namespace std;\n\nvoid solve() {\n        int\
    \ Q;\n        cin >> Q;\n        // minimize resizing\n        HashMap<int64_t,\
    \ int64_t> hash_map(Q);\n        while (Q--) {\n                int t;\n     \
    \           cin >> t;\n                if (!t) {\n                        int64_t\
    \ k, v;\n                        cin >> k >> v;\n                        hash_map[k]\
    \ = v;\n                } else {\n                        int64_t k;\n       \
    \                 cin >> k;\n                        cout << hash_map[k] << '\\\
    n';\n                }\n        }\n}\n\nint main() {\n        ios::sync_with_stdio(false);\n\
    \        cin.tie(NULL);\n\n        int t = 1;\n        // cin >> t;\n        while\
    \ (t--) solve();\n\n        return 0;\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/associative_array\"\n#include\
    \ \"ds/hashmap.hpp\"\n#include <bits/stdc++.h>\nusing namespace std;\n\nvoid solve()\
    \ {\n        int Q;\n        cin >> Q;\n        // minimize resizing\n       \
    \ HashMap<int64_t, int64_t> hash_map(Q);\n        while (Q--) {\n            \
    \    int t;\n                cin >> t;\n                if (!t) {\n          \
    \              int64_t k, v;\n                        cin >> k >> v;\n       \
    \                 hash_map[k] = v;\n                } else {\n               \
    \         int64_t k;\n                        cin >> k;\n                    \
    \    cout << hash_map[k] << '\\n';\n                }\n        }\n}\n\nint main()\
    \ {\n        ios::sync_with_stdio(false);\n        cin.tie(NULL);\n\n        int\
    \ t = 1;\n        // cin >> t;\n        while (t--) solve();\n\n        return\
    \ 0;\n}\n"
  dependsOn:
  - ds/hashmap.hpp
  isVerificationFile: true
  path: test/1_library_checker/ds/hashmap.test.cpp
  requiredBy: []
  timestamp: '2026-01-20 09:39:34+07:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/1_library_checker/ds/hashmap.test.cpp
layout: document
redirect_from:
- /verify/test/1_library_checker/ds/hashmap.test.cpp
- /verify/test/1_library_checker/ds/hashmap.test.cpp.html
title: test/1_library_checker/ds/hashmap.test.cpp
---
