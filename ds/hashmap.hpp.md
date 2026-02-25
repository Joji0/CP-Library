---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/1_library_checker/ds/hashmap.test.cpp
    title: test/1_library_checker/ds/hashmap.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/3197.test.cpp
    title: test/2_yukicoder/3197.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links:
    - http://xorshift.di.unimi.it/splitmix64.c
  bundledCode: "#line 2 \"ds/hashmap.hpp\"\n#include <cassert>\n#include <chrono>\n\
    #include <cstdint>\n#include <vector>\n\ntemplate <typename K, typename V> struct\
    \ HashMap {\n      private:\n        struct Node {\n                K key;\n \
    \               V val;\n                bool exists = false;\n        };\n   \
    \     std::vector<Node> table;\n        uint32_t cap, size;\n        uint64_t\
    \ random_seed;\n        static uint64_t splitmix64(uint64_t x) {\n           \
    \     // http://xorshift.di.unimi.it/splitmix64.c\n                x += 0x9e3779b97f4a7c15;\n\
    \                x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;\n                x\
    \ = (x ^ (x >> 27)) * 0x94d049bb133111eb;\n                return x ^ (x >> 31);\n\
    \        }\n        uint32_t get_index(K key) {\n                uint64_t hash\
    \ = splitmix64((uint64_t)key + random_seed);\n                return hash & (cap\
    \ - 1);\n        }\n        void expand() {\n                std::vector<Node>\
    \ old_table = table;\n                build(cap << 1);\n                for (const\
    \ auto &node : old_table) {\n                        if (node.exists) {\n    \
    \                            uint32_t idx = get_index(node.key);\n           \
    \                     while (table[idx].exists) {\n                          \
    \              idx++;\n                                        idx &= (cap - 1);\n\
    \                                }\n                                table[idx]\
    \ = {node.key, node.val, true};\n                                size++;\n   \
    \                     }\n                }\n        }\n\n      public:\n     \
    \   HashMap(uint32_t n = 1 << 20) {\n                uint32_t pw = 1;\n      \
    \          while (pw < n) pw <<= 1;\n                build(pw);\n        }\n \
    \       void build(uint32_t n) {\n                cap = n;\n                size\
    \ = 0;\n                table.assign(cap, {});\n                random_seed =\
    \ std::chrono::steady_clock::now().time_since_epoch().count();\n        }\n  \
    \      void insert(K key, V val) { (*this)[key] = val; }\n        V &operator[](K\
    \ key) {\n                if (size >= cap / 2) expand();\n                uint32_t\
    \ idx = get_index(key);\n                while (table[idx].exists) {\n       \
    \                 if (table[idx].key == key) {\n                             \
    \   return table[idx].val;\n                        }\n                      \
    \  idx++;\n                        idx &= (cap - 1);\n                }\n    \
    \            table[idx] = {key, V{}, true};\n                size++;\n       \
    \         return table[idx].val;\n        }\n        V get(K key, V default_val\
    \ = -1) {\n                uint32_t idx = get_index(key);\n                while\
    \ (table[idx].exists) {\n                        if (table[idx].key == key) {\n\
    \                                return table[idx].val;\n                    \
    \    }\n                        idx++;\n                        idx &= (cap -\
    \ 1);\n                }\n                return default_val;\n        }\n   \
    \     int sz() { return size; }\n};\n"
  code: "#pragma once\n#include <cassert>\n#include <chrono>\n#include <cstdint>\n\
    #include <vector>\n\ntemplate <typename K, typename V> struct HashMap {\n    \
    \  private:\n        struct Node {\n                K key;\n                V\
    \ val;\n                bool exists = false;\n        };\n        std::vector<Node>\
    \ table;\n        uint32_t cap, size;\n        uint64_t random_seed;\n       \
    \ static uint64_t splitmix64(uint64_t x) {\n                // http://xorshift.di.unimi.it/splitmix64.c\n\
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
    \     int sz() { return size; }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: ds/hashmap.hpp
  requiredBy: []
  timestamp: '2026-01-20 09:39:34+07:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/2_yukicoder/3197.test.cpp
  - test/1_library_checker/ds/hashmap.test.cpp
documentation_of: ds/hashmap.hpp
layout: document
redirect_from:
- /library/ds/hashmap.hpp
- /library/ds/hashmap.hpp.html
title: ds/hashmap.hpp
---
