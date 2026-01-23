---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: mod/dynamic_modint.hpp
    title: mod/dynamic_modint.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/3227.test.cpp
    title: test/2_yukicoder/3227.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links:
    - https://maspypy.github.io/library/mod/barrett.hpp
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
    \ b) const { return modulo((uint64_t)(a)*b); }\n};\n"
  code: "#pragma once\n#include <cstdint>\n#include <utility>\n\n// reference: https://maspypy.github.io/library/mod/barrett.hpp\n\
    struct Barrett {\n        uint32_t mod;\n        uint64_t inv_mod;\n        explicit\
    \ Barrett(uint32_t m = 1) : mod(m), inv_mod((uint64_t)(-1) / m + 1) {}\n     \
    \   uint32_t umod() const { return mod; }\n        uint32_t modulo(uint64_t z)\
    \ const {\n                if (mod == 1) return 0;\n                uint64_t x\
    \ = (uint64_t)(((__uint128_t)(z)*inv_mod) >> 64);\n                uint64_t y\
    \ = x * mod;\n                return (uint32_t)(z - y + (z < y ? mod : 0));\n\
    \        }\n        uint64_t floor(uint64_t z) const {\n                if (mod\
    \ == 1) return z;\n                uint64_t x = (uint64_t)(((__uint128_t)(z)*inv_mod)\
    \ >> 64);\n                uint64_t y = x * mod;\n                return (z <\
    \ y ? x - 1 : x);\n        }\n        std::pair<uint64_t, uint32_t> divmod(uint64_t\
    \ z) const {\n                if (mod == 1) return {z, 0};\n                uint64_t\
    \ x = (uint64_t)(((__uint128_t)(z)*inv_mod) >> 64);\n                uint64_t\
    \ y = x * mod;\n                if (z < y) return {x - 1, z - y + mod};\n    \
    \            return {x, z - y};\n        }\n        uint32_t mul(uint32_t a, uint32_t\
    \ b) const { return modulo((uint64_t)(a)*b); }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: mod/barrett.hpp
  requiredBy:
  - mod/dynamic_modint.hpp
  timestamp: '2026-01-21 20:36:29+07:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/2_yukicoder/3227.test.cpp
documentation_of: mod/barrett.hpp
layout: document
title: Barrett Reduction
---

# Barrett Reduction

Barrett Reduction is used to do modular arithmetic for some non-constant modulo fast. This requires some mathematical observation which can be seen in [Wikipedia](https://en.wikipedia.org/wiki/Barrett_reduction). The implementation used here is due to [Maspy](https://maspypy.github.io/library/mod/barrett.hpp) and [ACL](https://github.com/atcoder/ac-library/blob/master/atcoder/internal_math.hpp). 

Given a number $z$ we want to answer $z\mod m$ fast. The naive implementation is to use $z - m(z / m)$ where $z/m$ is the floor division in some programming language. Doing this repetitively will cost us lots of time due to the inefficiency of division compared to that of the multiplication. The idea of Barrett Reduction is to instead find an approximation of $z / m$. Indeed we can choose some very huge number that will be beneficial for the computer, say $2 ^ {64}$. We will then compute $\mathrm{im} = \frac{2^{64}}{m}$ and represent $z$ as

$$x \approx z\cdot\frac{\text{im}}{2^{64}}$$

where $x$ is the quotient $z / m$. Notice that $\mathrm{im}$ can be calculated only once (so we can think of it as precomputation) and use this number everytime we need it without doing inefficient division again. The advantage of this reformulation is that dividing $\mathrm{im}$ with $2^{64}$ is equivalent with shifting 64 bit to the right, which can be done really fast by the CPU. 
