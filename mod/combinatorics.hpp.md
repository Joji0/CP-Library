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
  bundledCode: "#line 2 \"mod/combinatorics.hpp\"\n#include <algorithm>\n#include\
    \ <cassert>\n#include <cstdint>\n#include <vector>\n\ntemplate <typename Mint>\
    \ struct Combinatorics {\n        struct Storage {\n                std::vector<Mint>\
    \ fact, inv, invfact, der;\n                Storage() {\n                    \
    \    fact = {Mint(1), Mint(1)};\n                        inv = {Mint(0), Mint(1)};\n\
    \                        invfact = {Mint(1), Mint(1)};\n                     \
    \   der = {Mint(1), Mint(0)};\n                }\n                void extend(int\
    \ n) {\n                        if (n < (int)fact.size()) return;\n          \
    \              int target = std::max(n + 1, (int)fact.size() * 2);\n         \
    \               int curr = (int)fact.size();\n                        int MOD\
    \ = Mint::mod();\n                        fact.resize(target);\n             \
    \           inv.resize(target);\n                        invfact.resize(target);\n\
    \                        der.resize(target);\n                        for (int\
    \ i = curr; i < target; i++) {\n                                fact[i] = fact[i\
    \ - 1] * Mint(i);\n                                if (i < MOD) {\n          \
    \                              inv[i] = inv[MOD % i] * Mint(MOD - MOD / i);\n\
    \                                } else {\n                                  \
    \      inv[i] = Mint(i).inverse();\n                                }\n      \
    \                          invfact[i] = invfact[i - 1] * inv[i];\n           \
    \                     der[i] = Mint(i - 1) * (der[i - 1] + der[i - 2]);\n    \
    \                    }\n                }\n        };\n        static Storage\
    \ &data() {\n                static Storage s;\n                return s;\n  \
    \      }\n        static void ensure(int n) { data().extend(n); }\n        static\
    \ Mint fact(int n) {\n                if (n < 0) return Mint(0);\n           \
    \     ensure(n);\n                return data().fact[n];\n        }\n        static\
    \ Mint inv(int n) {\n                if (n < 0) return Mint(n).inverse();\n  \
    \              ensure(n);\n                return data().inv[n];\n        }\n\
    \        static Mint invfact(int n) {\n                if (n < 0) return Mint(0);\n\
    \                ensure(n);\n                return data().invfact[n];\n     \
    \   }\n        static Mint derangement(int n) {\n                if (n < 0) return\
    \ Mint(0);\n                ensure(n);\n                return data().der[n];\n\
    \        }\n        static Mint C(int64_t n, int64_t k) {\n                if\
    \ (k < 0 || k > n) return Mint(0);\n                if (n < (int64_t)data().fact.size())\
    \ {\n                        return fact(n) * invfact(k) * invfact(n - k);\n \
    \               }\n                if (n < 20000000) {\n                     \
    \   ensure(n);\n                        return fact(n) * invfact(k) * invfact(n\
    \ - k);\n                }\n                return C_large(n, k);\n        }\n\
    \        static Mint C_large(int64_t n, int64_t k) {\n                if (k <\
    \ 0 || k > n) return Mint(0);\n                k = std::min(k, n - k);\n     \
    \           Mint num(1);\n                for (int64_t i = 0; i < k; i++) {\n\
    \                        num *= Mint(n - i);\n                }\n            \
    \    return num * invfact(k);\n        }\n        static Mint P(int64_t n, int64_t\
    \ k) {\n                if (k < 0 || k > n) return Mint(0);\n                if\
    \ (n < 20000000) {\n                        ensure(n);\n                     \
    \   return fact(n) * invfact(n - k);\n                }\n                Mint\
    \ res(1);\n                for (int64_t i = 0; i < k; i++) {\n               \
    \         res *= Mint(n - i);\n                }\n                return res;\n\
    \        }\n        static Mint H(int64_t n, int64_t k) {\n                if\
    \ (n < 0 || k < 0) return Mint(0);\n                if (n == 0 && k == 0) return\
    \ Mint(1);\n                return C(n + k - 1, k);\n        }\n        static\
    \ Mint stars_and_bars(int64_t n, int64_t k, bool allow_empty = true) {\n     \
    \           if (k <= 0) return (n == 0 && k == 0) ? Mint(1) : Mint(0);\n     \
    \           if (allow_empty) {\n                        return C(n + k - 1, k\
    \ - 1);\n                } else {\n                        if (n < k) return Mint(0);\n\
    \                        return C(n - 1, k - 1);\n                }\n        }\n\
    \        static Mint catalan(int n) {\n                if (n < 0) return Mint(0);\n\
    \                return C(2 * n, n) * inv(n + 1);\n        }\n        static Mint\
    \ multinomial(int n, const std::vector<int> &ks) {\n                if (n < 0)\
    \ return Mint(0);\n                int64_t sum = 0;\n                Mint denom(1);\n\
    \                for (int k : ks) {\n                        if (k < 0) return\
    \ Mint(0);\n                        sum += k;\n                        denom *=\
    \ invfact(k);\n                }\n                if (sum != n) return Mint(0);\n\
    \                return fact(n) * denom;\n        }\n};\n"
  code: "#pragma once\n#include <algorithm>\n#include <cassert>\n#include <cstdint>\n\
    #include <vector>\n\ntemplate <typename Mint> struct Combinatorics {\n       \
    \ struct Storage {\n                std::vector<Mint> fact, inv, invfact, der;\n\
    \                Storage() {\n                        fact = {Mint(1), Mint(1)};\n\
    \                        inv = {Mint(0), Mint(1)};\n                        invfact\
    \ = {Mint(1), Mint(1)};\n                        der = {Mint(1), Mint(0)};\n \
    \               }\n                void extend(int n) {\n                    \
    \    if (n < (int)fact.size()) return;\n                        int target = std::max(n\
    \ + 1, (int)fact.size() * 2);\n                        int curr = (int)fact.size();\n\
    \                        int MOD = Mint::mod();\n                        fact.resize(target);\n\
    \                        inv.resize(target);\n                        invfact.resize(target);\n\
    \                        der.resize(target);\n                        for (int\
    \ i = curr; i < target; i++) {\n                                fact[i] = fact[i\
    \ - 1] * Mint(i);\n                                if (i < MOD) {\n          \
    \                              inv[i] = inv[MOD % i] * Mint(MOD - MOD / i);\n\
    \                                } else {\n                                  \
    \      inv[i] = Mint(i).inverse();\n                                }\n      \
    \                          invfact[i] = invfact[i - 1] * inv[i];\n           \
    \                     der[i] = Mint(i - 1) * (der[i - 1] + der[i - 2]);\n    \
    \                    }\n                }\n        };\n        static Storage\
    \ &data() {\n                static Storage s;\n                return s;\n  \
    \      }\n        static void ensure(int n) { data().extend(n); }\n        static\
    \ Mint fact(int n) {\n                if (n < 0) return Mint(0);\n           \
    \     ensure(n);\n                return data().fact[n];\n        }\n        static\
    \ Mint inv(int n) {\n                if (n < 0) return Mint(n).inverse();\n  \
    \              ensure(n);\n                return data().inv[n];\n        }\n\
    \        static Mint invfact(int n) {\n                if (n < 0) return Mint(0);\n\
    \                ensure(n);\n                return data().invfact[n];\n     \
    \   }\n        static Mint derangement(int n) {\n                if (n < 0) return\
    \ Mint(0);\n                ensure(n);\n                return data().der[n];\n\
    \        }\n        static Mint C(int64_t n, int64_t k) {\n                if\
    \ (k < 0 || k > n) return Mint(0);\n                if (n < (int64_t)data().fact.size())\
    \ {\n                        return fact(n) * invfact(k) * invfact(n - k);\n \
    \               }\n                if (n < 20000000) {\n                     \
    \   ensure(n);\n                        return fact(n) * invfact(k) * invfact(n\
    \ - k);\n                }\n                return C_large(n, k);\n        }\n\
    \        static Mint C_large(int64_t n, int64_t k) {\n                if (k <\
    \ 0 || k > n) return Mint(0);\n                k = std::min(k, n - k);\n     \
    \           Mint num(1);\n                for (int64_t i = 0; i < k; i++) {\n\
    \                        num *= Mint(n - i);\n                }\n            \
    \    return num * invfact(k);\n        }\n        static Mint P(int64_t n, int64_t\
    \ k) {\n                if (k < 0 || k > n) return Mint(0);\n                if\
    \ (n < 20000000) {\n                        ensure(n);\n                     \
    \   return fact(n) * invfact(n - k);\n                }\n                Mint\
    \ res(1);\n                for (int64_t i = 0; i < k; i++) {\n               \
    \         res *= Mint(n - i);\n                }\n                return res;\n\
    \        }\n        static Mint H(int64_t n, int64_t k) {\n                if\
    \ (n < 0 || k < 0) return Mint(0);\n                if (n == 0 && k == 0) return\
    \ Mint(1);\n                return C(n + k - 1, k);\n        }\n        static\
    \ Mint stars_and_bars(int64_t n, int64_t k, bool allow_empty = true) {\n     \
    \           if (k <= 0) return (n == 0 && k == 0) ? Mint(1) : Mint(0);\n     \
    \           if (allow_empty) {\n                        return C(n + k - 1, k\
    \ - 1);\n                } else {\n                        if (n < k) return Mint(0);\n\
    \                        return C(n - 1, k - 1);\n                }\n        }\n\
    \        static Mint catalan(int n) {\n                if (n < 0) return Mint(0);\n\
    \                return C(2 * n, n) * inv(n + 1);\n        }\n        static Mint\
    \ multinomial(int n, const std::vector<int> &ks) {\n                if (n < 0)\
    \ return Mint(0);\n                int64_t sum = 0;\n                Mint denom(1);\n\
    \                for (int k : ks) {\n                        if (k < 0) return\
    \ Mint(0);\n                        sum += k;\n                        denom *=\
    \ invfact(k);\n                }\n                if (sum != n) return Mint(0);\n\
    \                return fact(n) * denom;\n        }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: mod/combinatorics.hpp
  requiredBy: []
  timestamp: '2026-01-22 09:48:12+07:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: mod/combinatorics.hpp
layout: document
redirect_from:
- /library/mod/combinatorics.hpp
- /library/mod/combinatorics.hpp.html
title: mod/combinatorics.hpp
---
