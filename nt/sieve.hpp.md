---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/144.test.cpp
    title: test/2_yukicoder/144.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"nt/sieve.hpp\"\n#include <cstdint>\n#include <vector>\n\n\
    namespace NT {\nstruct Sieve {\n        int N;\n        std::vector<int> primes;\n\
    \        std::vector<int> spf;\n        std::vector<int> phi;\n        std::vector<int>\
    \ mu;\n        Sieve(int max_n) : N(max_n), spf(max_n + 1, 0), phi(max_n + 1),\
    \ mu(max_n + 1) {\n                phi[1] = 1;\n                mu[1] = 1;\n \
    \               for (int i = 2; i <= N; i++) {\n                        if (spf[i]\
    \ == 0) {\n                                spf[i] = i;\n                     \
    \           primes.push_back(i);\n                                phi[i] = i -\
    \ 1;\n                                mu[i] = -1;\n                        }\n\
    \                        for (int p : primes) {\n                            \
    \    if (p > spf[i] || (int64_t)i * p > N) break;\n                          \
    \      spf[i * p] = p;\n                                if (spf[i] == p) {\n \
    \                                       phi[i * p] = phi[i] * p;\n           \
    \                             mu[i * p] = 0;\n                               \
    \         break;\n                                } else {\n                 \
    \                       phi[i * p] = phi[i] * (p - 1);\n                     \
    \                   mu[i * p] = -mu[i];\n                                }\n \
    \                       }\n                }\n        }\n        bool is_prime(int\
    \ x) const {\n                if (x <= 1 || x > N) return false;\n           \
    \     return spf[x] == x;\n        }\n        std::vector<std::pair<int, int>>\
    \ get_prime_factors(int x) const {\n                std::vector<std::pair<int,\
    \ int>> factors;\n                while (x > 1) {\n                        int\
    \ p = spf[x];\n                        int exponent = 0;\n                   \
    \     while (x % p == 0) {\n                                x /= p;\n        \
    \                        exponent++;\n                        }\n            \
    \            factors.push_back({p, exponent});\n                }\n          \
    \      return factors;\n        }\n        std::vector<int> get_distinct_primes(int\
    \ x) const {\n                std::vector<int> distinct;\n                while\
    \ (x > 1) {\n                        int p = spf[x];\n                       \
    \ distinct.push_back(p);\n                        while (x % p == 0) x /= p;\n\
    \                }\n                return distinct;\n        }\n        int64_t\
    \ count_divisors(int x) const {\n                if (x == 1) return 1;\n     \
    \           int64_t res = 1;\n                while (x > 1) {\n              \
    \          int p = spf[x];\n                        int e = 0;\n             \
    \           while (x % p == 0) {\n                                x /= p;\n  \
    \                              e++;\n                        }\n             \
    \           res *= (e + 1);\n                }\n                return res;\n\
    \        }\n        int64_t sum_divisors(int x) const {\n                if (x\
    \ == 1) return 1;\n                int64_t res = 1;\n                while (x\
    \ > 1) {\n                        int p = spf[x];\n                        int64_t\
    \ sum_p = 1, p_pow = 1;\n                        while (x % p == 0) {\n      \
    \                          x /= p;\n                                p_pow *= p;\n\
    \                                sum_p += p_pow;\n                        }\n\
    \                        res *= sum_p;\n                }\n                return\
    \ res;\n        }\n        std::vector<int64_t> get_all_divisors(int x) const\
    \ {\n                auto factors = get_prime_factors(x);\n                std::vector<int64_t>\
    \ divs = {1};\n                for (auto &pf : factors) {\n                  \
    \      int p = pf.first;\n                        int count = pf.second;\n   \
    \                     int sz = divs.size();\n                        int64_t cur_p\
    \ = 1;\n                        for (int i = 0; i < count; ++i) {\n          \
    \                      cur_p *= p;\n                                for (int j\
    \ = 0; j < sz; ++j) {\n                                        divs.push_back(divs[j]\
    \ * cur_p);\n                                }\n                        }\n  \
    \              }\n                return divs;\n        }\n};\n} // namespace\
    \ NT\n"
  code: "#pragma once\n#include <cstdint>\n#include <vector>\n\nnamespace NT {\nstruct\
    \ Sieve {\n        int N;\n        std::vector<int> primes;\n        std::vector<int>\
    \ spf;\n        std::vector<int> phi;\n        std::vector<int> mu;\n        Sieve(int\
    \ max_n) : N(max_n), spf(max_n + 1, 0), phi(max_n + 1), mu(max_n + 1) {\n    \
    \            phi[1] = 1;\n                mu[1] = 1;\n                for (int\
    \ i = 2; i <= N; i++) {\n                        if (spf[i] == 0) {\n        \
    \                        spf[i] = i;\n                                primes.push_back(i);\n\
    \                                phi[i] = i - 1;\n                           \
    \     mu[i] = -1;\n                        }\n                        for (int\
    \ p : primes) {\n                                if (p > spf[i] || (int64_t)i\
    \ * p > N) break;\n                                spf[i * p] = p;\n         \
    \                       if (spf[i] == p) {\n                                 \
    \       phi[i * p] = phi[i] * p;\n                                        mu[i\
    \ * p] = 0;\n                                        break;\n                \
    \                } else {\n                                        phi[i * p]\
    \ = phi[i] * (p - 1);\n                                        mu[i * p] = -mu[i];\n\
    \                                }\n                        }\n              \
    \  }\n        }\n        bool is_prime(int x) const {\n                if (x <=\
    \ 1 || x > N) return false;\n                return spf[x] == x;\n        }\n\
    \        std::vector<std::pair<int, int>> get_prime_factors(int x) const {\n \
    \               std::vector<std::pair<int, int>> factors;\n                while\
    \ (x > 1) {\n                        int p = spf[x];\n                       \
    \ int exponent = 0;\n                        while (x % p == 0) {\n          \
    \                      x /= p;\n                                exponent++;\n\
    \                        }\n                        factors.push_back({p, exponent});\n\
    \                }\n                return factors;\n        }\n        std::vector<int>\
    \ get_distinct_primes(int x) const {\n                std::vector<int> distinct;\n\
    \                while (x > 1) {\n                        int p = spf[x];\n  \
    \                      distinct.push_back(p);\n                        while (x\
    \ % p == 0) x /= p;\n                }\n                return distinct;\n   \
    \     }\n        int64_t count_divisors(int x) const {\n                if (x\
    \ == 1) return 1;\n                int64_t res = 1;\n                while (x\
    \ > 1) {\n                        int p = spf[x];\n                        int\
    \ e = 0;\n                        while (x % p == 0) {\n                     \
    \           x /= p;\n                                e++;\n                  \
    \      }\n                        res *= (e + 1);\n                }\n       \
    \         return res;\n        }\n        int64_t sum_divisors(int x) const {\n\
    \                if (x == 1) return 1;\n                int64_t res = 1;\n   \
    \             while (x > 1) {\n                        int p = spf[x];\n     \
    \                   int64_t sum_p = 1, p_pow = 1;\n                        while\
    \ (x % p == 0) {\n                                x /= p;\n                  \
    \              p_pow *= p;\n                                sum_p += p_pow;\n\
    \                        }\n                        res *= sum_p;\n          \
    \      }\n                return res;\n        }\n        std::vector<int64_t>\
    \ get_all_divisors(int x) const {\n                auto factors = get_prime_factors(x);\n\
    \                std::vector<int64_t> divs = {1};\n                for (auto &pf\
    \ : factors) {\n                        int p = pf.first;\n                  \
    \      int count = pf.second;\n                        int sz = divs.size();\n\
    \                        int64_t cur_p = 1;\n                        for (int\
    \ i = 0; i < count; ++i) {\n                                cur_p *= p;\n    \
    \                            for (int j = 0; j < sz; ++j) {\n                \
    \                        divs.push_back(divs[j] * cur_p);\n                  \
    \              }\n                        }\n                }\n             \
    \   return divs;\n        }\n};\n} // namespace NT\n"
  dependsOn: []
  isVerificationFile: false
  path: nt/sieve.hpp
  requiredBy: []
  timestamp: '2026-01-23 14:06:38+07:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/2_yukicoder/144.test.cpp
documentation_of: nt/sieve.hpp
layout: document
redirect_from:
- /library/nt/sieve.hpp
- /library/nt/sieve.hpp.html
title: nt/sieve.hpp
---
