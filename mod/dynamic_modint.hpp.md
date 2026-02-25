---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: mod/barrett.hpp
    title: Barrett Reduction
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/3227.test.cpp
    title: test/2_yukicoder/3227.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"mod/barrett.hpp\"\n#include <cstdint>\n#include <utility>\n\
    \n// reference: https://maspypy.github.io/library/mod/barrett.hpp\nstruct Barrett\
    \ {\n        uint32_t mod;\n        uint64_t inv_mod;\n        explicit Barrett(uint32_t\
    \ m = 1) : mod(m), inv_mod((uint64_t)(-1) / m + 1) {}\n        uint32_t umod()\
    \ const { return mod; }\n        uint32_t modulo(uint64_t z) const {\n       \
    \         if (mod == 1) return 0;\n                uint64_t x = (uint64_t)(((__uint128_t)(z)*inv_mod)\
    \ >> 64);\n                uint64_t y = x * mod;\n                return (uint32_t)(z\
    \ - y + (z < y ? mod : 0));\n        }\n        uint64_t floor(uint64_t z) const\
    \ {\n                if (mod == 1) return z;\n                uint64_t x = (uint64_t)(((__uint128_t)(z)*inv_mod)\
    \ >> 64);\n                uint64_t y = x * mod;\n                return (z <\
    \ y ? x - 1 : x);\n        }\n        std::pair<uint64_t, uint32_t> divmod(uint64_t\
    \ z) const {\n                if (mod == 1) return {z, 0};\n                uint64_t\
    \ x = (uint64_t)(((__uint128_t)(z)*inv_mod) >> 64);\n                uint64_t\
    \ y = x * mod;\n                if (z < y) return {x - 1, z - y + mod};\n    \
    \            return {x, z - y};\n        }\n        uint32_t mul(uint32_t a, uint32_t\
    \ b) const { return modulo((uint64_t)(a)*b); }\n};\n#line 3 \"mod/dynamic_modint.hpp\"\
    \n#include <cassert>\n#line 5 \"mod/dynamic_modint.hpp\"\n#include <iostream>\n\
    #line 7 \"mod/dynamic_modint.hpp\"\n\ntemplate <int id> struct DynamicModInt {\n\
    \        static inline Barrett MOD = Barrett(998244353);\n        uint32_t val;\n\
    \        static void set_mod(uint32_t m) {\n                assert(m > 0);\n \
    \               MOD = Barrett(m);\n        }\n        static uint32_t mod() {\
    \ return MOD.umod(); }\n        DynamicModInt() : val(0) {}\n        DynamicModInt(const\
    \ int64_t &x)\n            : val(x >= 0 ? x % MOD.umod() : (MOD.umod() - (-x)\
    \ % MOD.umod()) % MOD.umod()) {}\n        uint32_t value() const { return val;\
    \ }\n        DynamicModInt &operator+=(const DynamicModInt &rhs) {\n         \
    \       val += rhs.val;\n                if (val >= MOD.umod()) val -= MOD.umod();\n\
    \                return *this;\n        }\n        DynamicModInt &operator-=(const\
    \ DynamicModInt &rhs) {\n                if (val < rhs.val) val += MOD.umod();\n\
    \                val -= rhs.val;\n                return *this;\n        }\n \
    \       DynamicModInt &operator*=(const DynamicModInt &rhs) {\n              \
    \  val = MOD.mul(val, rhs.val);\n                return *this;\n        }\n  \
    \      DynamicModInt &operator/=(const DynamicModInt &rhs) { return *this *= rhs.inverse();\
    \ }\n        DynamicModInt operator+() const { return *this; }\n        DynamicModInt\
    \ operator-() const { return DynamicModInt(val == 0 ? 0 : MOD.umod() - val); }\n\
    \        friend DynamicModInt operator+(DynamicModInt lhs, const DynamicModInt\
    \ &rhs) { return lhs += rhs; }\n        friend DynamicModInt operator-(DynamicModInt\
    \ lhs, const DynamicModInt &rhs) { return lhs -= rhs; }\n        friend DynamicModInt\
    \ operator*(DynamicModInt lhs, const DynamicModInt &rhs) { return lhs *= rhs;\
    \ }\n        friend DynamicModInt operator/(DynamicModInt lhs, const DynamicModInt\
    \ &rhs) { return lhs /= rhs; }\n        friend bool operator==(const DynamicModInt\
    \ &lhs, const DynamicModInt &rhs) { return lhs.val == rhs.val; }\n        friend\
    \ bool operator!=(const DynamicModInt &lhs, const DynamicModInt &rhs) { return\
    \ lhs.val != rhs.val; }\n        DynamicModInt pow(uint64_t n) const {\n     \
    \           DynamicModInt res = 1, a = *this;\n                while (n > 0) {\n\
    \                        if (n & 1) res *= a;\n                        a *= a;\n\
    \                        n >>= 1;\n                }\n                return res;\n\
    \        }\n        DynamicModInt inverse() const {\n                int64_t a\
    \ = val, b = MOD.umod(), u = 1, v = 0;\n                while (b) {\n        \
    \                int64_t t = a / b;\n                        a -= t * b;\n   \
    \                     std::swap(a, b);\n                        u -= t * v;\n\
    \                        std::swap(u, v);\n                }\n               \
    \ return DynamicModInt(u);\n        }\n        friend std::ostream &operator<<(std::ostream\
    \ &os, const DynamicModInt &x) { return os << x.val; }\n        friend std::istream\
    \ &operator>>(std::istream &is, DynamicModInt &x) {\n                int64_t v;\n\
    \                is >> v;\n                x = DynamicModInt(v);\n           \
    \     return is;\n        }\n};\n"
  code: "#pragma once\n#include \"mod/barrett.hpp\"\n#include <cassert>\n#include\
    \ <cstdint>\n#include <iostream>\n#include <utility>\n\ntemplate <int id> struct\
    \ DynamicModInt {\n        static inline Barrett MOD = Barrett(998244353);\n \
    \       uint32_t val;\n        static void set_mod(uint32_t m) {\n           \
    \     assert(m > 0);\n                MOD = Barrett(m);\n        }\n        static\
    \ uint32_t mod() { return MOD.umod(); }\n        DynamicModInt() : val(0) {}\n\
    \        DynamicModInt(const int64_t &x)\n            : val(x >= 0 ? x % MOD.umod()\
    \ : (MOD.umod() - (-x) % MOD.umod()) % MOD.umod()) {}\n        uint32_t value()\
    \ const { return val; }\n        DynamicModInt &operator+=(const DynamicModInt\
    \ &rhs) {\n                val += rhs.val;\n                if (val >= MOD.umod())\
    \ val -= MOD.umod();\n                return *this;\n        }\n        DynamicModInt\
    \ &operator-=(const DynamicModInt &rhs) {\n                if (val < rhs.val)\
    \ val += MOD.umod();\n                val -= rhs.val;\n                return\
    \ *this;\n        }\n        DynamicModInt &operator*=(const DynamicModInt &rhs)\
    \ {\n                val = MOD.mul(val, rhs.val);\n                return *this;\n\
    \        }\n        DynamicModInt &operator/=(const DynamicModInt &rhs) { return\
    \ *this *= rhs.inverse(); }\n        DynamicModInt operator+() const { return\
    \ *this; }\n        DynamicModInt operator-() const { return DynamicModInt(val\
    \ == 0 ? 0 : MOD.umod() - val); }\n        friend DynamicModInt operator+(DynamicModInt\
    \ lhs, const DynamicModInt &rhs) { return lhs += rhs; }\n        friend DynamicModInt\
    \ operator-(DynamicModInt lhs, const DynamicModInt &rhs) { return lhs -= rhs;\
    \ }\n        friend DynamicModInt operator*(DynamicModInt lhs, const DynamicModInt\
    \ &rhs) { return lhs *= rhs; }\n        friend DynamicModInt operator/(DynamicModInt\
    \ lhs, const DynamicModInt &rhs) { return lhs /= rhs; }\n        friend bool operator==(const\
    \ DynamicModInt &lhs, const DynamicModInt &rhs) { return lhs.val == rhs.val; }\n\
    \        friend bool operator!=(const DynamicModInt &lhs, const DynamicModInt\
    \ &rhs) { return lhs.val != rhs.val; }\n        DynamicModInt pow(uint64_t n)\
    \ const {\n                DynamicModInt res = 1, a = *this;\n               \
    \ while (n > 0) {\n                        if (n & 1) res *= a;\n            \
    \            a *= a;\n                        n >>= 1;\n                }\n  \
    \              return res;\n        }\n        DynamicModInt inverse() const {\n\
    \                int64_t a = val, b = MOD.umod(), u = 1, v = 0;\n            \
    \    while (b) {\n                        int64_t t = a / b;\n               \
    \         a -= t * b;\n                        std::swap(a, b);\n            \
    \            u -= t * v;\n                        std::swap(u, v);\n         \
    \       }\n                return DynamicModInt(u);\n        }\n        friend\
    \ std::ostream &operator<<(std::ostream &os, const DynamicModInt &x) { return\
    \ os << x.val; }\n        friend std::istream &operator>>(std::istream &is, DynamicModInt\
    \ &x) {\n                int64_t v;\n                is >> v;\n              \
    \  x = DynamicModInt(v);\n                return is;\n        }\n};\n"
  dependsOn:
  - mod/barrett.hpp
  isVerificationFile: false
  path: mod/dynamic_modint.hpp
  requiredBy: []
  timestamp: '2026-01-22 09:44:52+07:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/2_yukicoder/3227.test.cpp
documentation_of: mod/dynamic_modint.hpp
layout: document
redirect_from:
- /library/mod/dynamic_modint.hpp
- /library/mod/dynamic_modint.hpp.html
title: mod/dynamic_modint.hpp
---
