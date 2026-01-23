---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/3227.test.cpp
    title: test/2_yukicoder/3227.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/3228.test.cpp
    title: test/2_yukicoder/3228.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"ds/matrix.hpp\"\n#include <cassert>\n#include <cstdint>\n\
    #include <vector>\n\ntemplate <typename T> struct Matrix {\n        int H, W;\n\
    \        std::vector<std::vector<T>> table;\n        Matrix(int h, int w) : H(h),\
    \ W(w), table(h, std::vector<T>(w)) {}\n        Matrix(const std::vector<std::vector<T>>\
    \ &v) : H((int)v.size()), W((int)v[0].size()), table(v) {}\n        std::vector<T>\
    \ &operator[](int i) { return table[i]; }\n        const std::vector<T> &operator[](int\
    \ i) const { return table[i]; }\n        static Matrix identity(int N) {\n   \
    \             Matrix res(N, N);\n                for (int i = 0; i < N; i++) {\n\
    \                        res[i][i] = 1;\n                }\n                return\
    \ res;\n        }\n        Matrix &operator+=(const Matrix &rhs) {\n         \
    \       assert(H == rhs.H && W == rhs.W && \"DIMENSION must be the same\");\n\
    \                for (int i = 0; i < H; i++) {\n                        for (int\
    \ j = 0; j < W; j++) {\n                                table[i][j] += rhs[i][j];\n\
    \                        }\n                }\n                return *this;\n\
    \        }\n        Matrix &operator-=(const Matrix &rhs) {\n                assert(H\
    \ == rhs.H && W == rhs.W && \"DIMENSION must be the same\");\n               \
    \ for (int i = 0; i < H; i++) {\n                        for (int j = 0; j < W;\
    \ j++) {\n                                table[i][j] -= rhs[i][j];\n        \
    \                }\n                }\n                return *this;\n       \
    \ }\n        Matrix operator*(const Matrix &rhs) const {\n                assert(W\
    \ == rhs.H && \"MULTIPLICATION DIMENSION does not match\");\n                Matrix\
    \ res(H, rhs.W);\n                for (int i = 0; i < H; i++) {\n            \
    \            for (int j = 0; j < W; j++) {\n                                if\
    \ (table[i][j] == 0) continue;\n                                for (int k = 0;\
    \ k < rhs.W; k++) {\n                                        res[i][k] += table[i][j]\
    \ * rhs[j][k];\n                                }\n                        }\n\
    \                }\n                return res;\n        }\n        Matrix &operator*=(const\
    \ Matrix &rhs) { return *this = *this * rhs; }\n        Matrix pow(int64_t n)\
    \ const {\n                assert(H == W && \"DIMENSION must be square\");\n \
    \               Matrix res = identity(H);\n                Matrix a = *this;\n\
    \                while (n > 0) {\n                        if (n & 1) res *= a;\n\
    \                        a *= a;\n                        n >>= 1;\n         \
    \       }\n                return res;\n        }\n};\n"
  code: "#pragma once\n#include <cassert>\n#include <cstdint>\n#include <vector>\n\
    \ntemplate <typename T> struct Matrix {\n        int H, W;\n        std::vector<std::vector<T>>\
    \ table;\n        Matrix(int h, int w) : H(h), W(w), table(h, std::vector<T>(w))\
    \ {}\n        Matrix(const std::vector<std::vector<T>> &v) : H((int)v.size()),\
    \ W((int)v[0].size()), table(v) {}\n        std::vector<T> &operator[](int i)\
    \ { return table[i]; }\n        const std::vector<T> &operator[](int i) const\
    \ { return table[i]; }\n        static Matrix identity(int N) {\n            \
    \    Matrix res(N, N);\n                for (int i = 0; i < N; i++) {\n      \
    \                  res[i][i] = 1;\n                }\n                return res;\n\
    \        }\n        Matrix &operator+=(const Matrix &rhs) {\n                assert(H\
    \ == rhs.H && W == rhs.W && \"DIMENSION must be the same\");\n               \
    \ for (int i = 0; i < H; i++) {\n                        for (int j = 0; j < W;\
    \ j++) {\n                                table[i][j] += rhs[i][j];\n        \
    \                }\n                }\n                return *this;\n       \
    \ }\n        Matrix &operator-=(const Matrix &rhs) {\n                assert(H\
    \ == rhs.H && W == rhs.W && \"DIMENSION must be the same\");\n               \
    \ for (int i = 0; i < H; i++) {\n                        for (int j = 0; j < W;\
    \ j++) {\n                                table[i][j] -= rhs[i][j];\n        \
    \                }\n                }\n                return *this;\n       \
    \ }\n        Matrix operator*(const Matrix &rhs) const {\n                assert(W\
    \ == rhs.H && \"MULTIPLICATION DIMENSION does not match\");\n                Matrix\
    \ res(H, rhs.W);\n                for (int i = 0; i < H; i++) {\n            \
    \            for (int j = 0; j < W; j++) {\n                                if\
    \ (table[i][j] == 0) continue;\n                                for (int k = 0;\
    \ k < rhs.W; k++) {\n                                        res[i][k] += table[i][j]\
    \ * rhs[j][k];\n                                }\n                        }\n\
    \                }\n                return res;\n        }\n        Matrix &operator*=(const\
    \ Matrix &rhs) { return *this = *this * rhs; }\n        Matrix pow(int64_t n)\
    \ const {\n                assert(H == W && \"DIMENSION must be square\");\n \
    \               Matrix res = identity(H);\n                Matrix a = *this;\n\
    \                while (n > 0) {\n                        if (n & 1) res *= a;\n\
    \                        a *= a;\n                        n >>= 1;\n         \
    \       }\n                return res;\n        }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: ds/matrix.hpp
  requiredBy: []
  timestamp: '2026-01-21 18:02:06+07:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/2_yukicoder/3227.test.cpp
  - test/2_yukicoder/3228.test.cpp
documentation_of: ds/matrix.hpp
layout: document
redirect_from:
- /library/ds/matrix.hpp
- /library/ds/matrix.hpp.html
title: ds/matrix.hpp
---
