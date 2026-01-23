---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/3228.test.cpp
    title: test/2_yukicoder/3228.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"mod/modint.hpp\"\n#include <cstdint>\n#include <iostream>\n\
    #include <utility>\n\ntemplate <uint32_t MOD> struct ModInt {\n        static_assert(MOD\
    \ > 1, \"MOD must be > 1\");\n        uint32_t val;\n        static uint32_t mod()\
    \ { return MOD; }\n        constexpr ModInt() : val(0) {}\n        constexpr ModInt(const\
    \ int64_t &x) : val(x >= 0 ? x % MOD : (MOD - (-x) % MOD) % MOD) {}\n        constexpr\
    \ uint32_t value() const { return val; }\n        constexpr ModInt &operator+=(const\
    \ ModInt &rhs) {\n                val += rhs.val;\n                if (val >=\
    \ MOD) val -= MOD;\n                return *this;\n        }\n        constexpr\
    \ ModInt &operator-=(const ModInt &rhs) {\n                if (val < rhs.val)\
    \ val += MOD;\n                val -= rhs.val;\n                return *this;\n\
    \        }\n        constexpr ModInt &operator*=(const ModInt &rhs) {\n      \
    \          val = (uint64_t)val * rhs.val % MOD;\n                return *this;\n\
    \        }\n        constexpr ModInt &operator/=(const ModInt &rhs) { return *this\
    \ *= rhs.inverse(); }\n        constexpr ModInt operator+() const { return *this;\
    \ }\n        constexpr ModInt operator-() const { return ModInt(val == 0 ? 0 :\
    \ MOD - val); }\n        friend constexpr ModInt operator+(ModInt lhs, const ModInt\
    \ &rhs) { return lhs += rhs; }\n        friend constexpr ModInt operator-(ModInt\
    \ lhs, const ModInt &rhs) { return lhs -= rhs; }\n        friend constexpr ModInt\
    \ operator*(ModInt lhs, const ModInt &rhs) { return lhs *= rhs; }\n        friend\
    \ constexpr ModInt operator/(ModInt lhs, const ModInt &rhs) { return lhs /= rhs;\
    \ }\n        friend constexpr bool operator==(const ModInt &lhs, const ModInt\
    \ &rhs) { return lhs.val == rhs.val; }\n        friend constexpr bool operator!=(const\
    \ ModInt &lhs, const ModInt &rhs) { return lhs.val != rhs.val; }\n        constexpr\
    \ ModInt pow(uint64_t n) const {\n                ModInt res = 1, a = *this;\n\
    \                while (n > 0) {\n                        if (n & 1) res *= a;\n\
    \                        a *= a;\n                        n >>= 1;\n         \
    \       }\n                return res;\n        }\n        constexpr ModInt inverse()\
    \ const {\n                int64_t a = val, b = MOD, u = 1, v = 0;\n         \
    \       while (b) {\n                        int64_t t = a / b;\n            \
    \            a -= t * b;\n                        std::swap(a, b);\n         \
    \               u -= t * v;\n                        std::swap(u, v);\n      \
    \          }\n                return ModInt(u);\n        }\n        friend std::ostream\
    \ &operator<<(std::ostream &os, const ModInt &x) { return os << x.val; }\n   \
    \     friend std::istream &operator>>(std::istream &is, ModInt &x) {\n       \
    \         int64_t v;\n                is >> v;\n                x = ModInt(v);\n\
    \                return is;\n        }\n};\n"
  code: "#pragma once\n#include <cstdint>\n#include <iostream>\n#include <utility>\n\
    \ntemplate <uint32_t MOD> struct ModInt {\n        static_assert(MOD > 1, \"MOD\
    \ must be > 1\");\n        uint32_t val;\n        static uint32_t mod() { return\
    \ MOD; }\n        constexpr ModInt() : val(0) {}\n        constexpr ModInt(const\
    \ int64_t &x) : val(x >= 0 ? x % MOD : (MOD - (-x) % MOD) % MOD) {}\n        constexpr\
    \ uint32_t value() const { return val; }\n        constexpr ModInt &operator+=(const\
    \ ModInt &rhs) {\n                val += rhs.val;\n                if (val >=\
    \ MOD) val -= MOD;\n                return *this;\n        }\n        constexpr\
    \ ModInt &operator-=(const ModInt &rhs) {\n                if (val < rhs.val)\
    \ val += MOD;\n                val -= rhs.val;\n                return *this;\n\
    \        }\n        constexpr ModInt &operator*=(const ModInt &rhs) {\n      \
    \          val = (uint64_t)val * rhs.val % MOD;\n                return *this;\n\
    \        }\n        constexpr ModInt &operator/=(const ModInt &rhs) { return *this\
    \ *= rhs.inverse(); }\n        constexpr ModInt operator+() const { return *this;\
    \ }\n        constexpr ModInt operator-() const { return ModInt(val == 0 ? 0 :\
    \ MOD - val); }\n        friend constexpr ModInt operator+(ModInt lhs, const ModInt\
    \ &rhs) { return lhs += rhs; }\n        friend constexpr ModInt operator-(ModInt\
    \ lhs, const ModInt &rhs) { return lhs -= rhs; }\n        friend constexpr ModInt\
    \ operator*(ModInt lhs, const ModInt &rhs) { return lhs *= rhs; }\n        friend\
    \ constexpr ModInt operator/(ModInt lhs, const ModInt &rhs) { return lhs /= rhs;\
    \ }\n        friend constexpr bool operator==(const ModInt &lhs, const ModInt\
    \ &rhs) { return lhs.val == rhs.val; }\n        friend constexpr bool operator!=(const\
    \ ModInt &lhs, const ModInt &rhs) { return lhs.val != rhs.val; }\n        constexpr\
    \ ModInt pow(uint64_t n) const {\n                ModInt res = 1, a = *this;\n\
    \                while (n > 0) {\n                        if (n & 1) res *= a;\n\
    \                        a *= a;\n                        n >>= 1;\n         \
    \       }\n                return res;\n        }\n        constexpr ModInt inverse()\
    \ const {\n                int64_t a = val, b = MOD, u = 1, v = 0;\n         \
    \       while (b) {\n                        int64_t t = a / b;\n            \
    \            a -= t * b;\n                        std::swap(a, b);\n         \
    \               u -= t * v;\n                        std::swap(u, v);\n      \
    \          }\n                return ModInt(u);\n        }\n        friend std::ostream\
    \ &operator<<(std::ostream &os, const ModInt &x) { return os << x.val; }\n   \
    \     friend std::istream &operator>>(std::istream &is, ModInt &x) {\n       \
    \         int64_t v;\n                is >> v;\n                x = ModInt(v);\n\
    \                return is;\n        }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: mod/modint.hpp
  requiredBy: []
  timestamp: '2026-01-22 09:44:52+07:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/2_yukicoder/3228.test.cpp
documentation_of: mod/modint.hpp
layout: document
redirect_from:
- /library/mod/modint.hpp
- /library/mod/modint.hpp.html
title: mod/modint.hpp
---
